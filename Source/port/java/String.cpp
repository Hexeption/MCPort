#include "String.h"

#include <type_traits>
#include <stdexcept>

namespace String {
    namespace {
        static void appendUtf8(std::string &out, char32_t codePoint) {
            if (codePoint <= 0x7F) {
                out.push_back(static_cast<char>(codePoint));
            } else if (codePoint <= 0x7FF) {
                out.push_back(static_cast<char>(0xC0 | (codePoint >> 6)));
                out.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
            } else if (codePoint <= 0xFFFF) {
                out.push_back(static_cast<char>(0xE0 | (codePoint >> 12)));
                out.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
                out.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
            } else {
                out.push_back(static_cast<char>(0xF0 | (codePoint >> 18)));
                out.push_back(static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F)));
                out.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
                out.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
            }
        }

        static char32_t decodeUtf8(const std::string &str, size_t &index) {
            const unsigned char lead = static_cast<unsigned char>(str[index++]);

            if (lead < 0x80) {
                return lead;
            }

            auto readContinuation = [&](char32_t &value) -> bool {
                if (index >= str.size()) {
                    return false;
                }

                const unsigned char ch = static_cast<unsigned char>(str[index]);
                if ((ch & 0xC0) != 0x80) {
                    return false;
                }

                value = (value << 6) | (ch & 0x3F);
                ++index;
                return true;
            };

            char32_t codePoint = 0;
            size_t remaining = 0;

            if ((lead & 0xE0) == 0xC0) {
                codePoint = lead & 0x1F;
                remaining = 1;
                if (codePoint < 0x2) {
                    throw std::range_error("Invalid UTF-8 sequence");
                }
            } else if ((lead & 0xF0) == 0xE0) {
                codePoint = lead & 0x0F;
                remaining = 2;
            } else if ((lead & 0xF8) == 0xF0) {
                codePoint = lead & 0x07;
                remaining = 3;
                if (codePoint > 0x04) {
                    throw std::range_error("Invalid UTF-8 sequence");
                }
            } else {
                throw std::range_error("Invalid UTF-8 sequence");
            }

            for (size_t i = 0; i < remaining; ++i) {
                if (!readContinuation(codePoint)) {
                    throw std::range_error("Invalid UTF-8 sequence");
                }
            }

            if ((remaining == 1 && codePoint < 0x80) ||
                (remaining == 2 && codePoint < 0x800) ||
                (remaining == 3 && codePoint < 0x10000) ||
                (codePoint >= 0xD800 && codePoint <= 0xDFFF) ||
                codePoint > 0x10FFFF) {
                throw std::range_error("Invalid UTF-8 sequence");
            }

            return codePoint;
        }
    }

    jstring fromUtf8(const std::string &str) {
        jstring result;

        for (size_t i = 0; i < str.size();) {
            const char32_t codePoint = decodeUtf8(str, i);
            if (codePoint <= 0xFFFF) {
                result.push_back(static_cast<char16_t>(codePoint));
            } else {
                const char32_t value = codePoint - 0x10000;
                result.push_back(static_cast<char16_t>(0xD800 | (value >> 10)));
                result.push_back(static_cast<char16_t>(0xDC00 | (value & 0x3FF)));
            }
        }

        return result;
    }

    std::string toUtf8(const jstring &str) {
        std::string result;
        result.reserve(str.size());

        for (size_t i = 0; i < str.size(); ++i) {
            const char32_t lead = str[i];
            char32_t codePoint = lead;

            if (lead >= 0xD800 && lead <= 0xDBFF) {
                if (i + 1 >= str.size()) {
                    throw std::range_error("Invalid UTF-16 sequence");
                }

                const char32_t trail = str[i + 1];
                if (trail < 0xDC00 || trail > 0xDFFF) {
                    throw std::range_error("Invalid UTF-16 sequence");
                }

                codePoint = 0x10000 + ((lead - 0xD800) << 10) + (trail - 0xDC00);
                ++i;
            } else if (lead >= 0xDC00 && lead <= 0xDFFF) {
                throw std::range_error("Invalid UTF-16 sequence");
            }

            appendUtf8(result, codePoint);
        }

        return result;
    }

    template<typename T>
    static jstring intToStringImpl(T v, int_t base) {
        jstring out;

        if (base < 2 || base > 36) {
            throw std::invalid_argument("base must be in the range [2, 36]");
        }

        using UnsignedT = std::make_unsigned_t<T>;
        bool negative = false;
        UnsignedT value;

        if constexpr (std::is_signed_v<T>) {
            negative = v < 0;
            value = negative ? static_cast<UnsignedT>(-(v + 1)) + 1 : static_cast<UnsignedT>(v);
        } else {
            value = v;
        }

        while (value) {
            const int_t digit = static_cast<int_t>(value % static_cast<UnsignedT>(base));
            value /= static_cast<UnsignedT>(base);
            out.insert(out.begin(), digit + (digit < 10 ? '0' : 'a' - 10));
        }

        if (out.empty())
            out.push_back('0');

        if (negative)
            out.insert(out.begin(), '-');

        return out;
    }

    jstring toString(int_t v, int_t base) {
        return intToStringImpl<int_t>(v, base);
    }

    jstring toString(long_t v, int_t base) {
        return intToStringImpl<long_t>(v, base);
    }

    jstring toString(uint_t v, int_t base) {
        return intToStringImpl<uint_t>(v, base);
    }

    jstring toString(ulong_t v, int_t base) {
        return intToStringImpl<ulong_t>(v, base);
    }

    jstring toString(float v) {
        return fromUtf8(std::to_string(v));
    }

    jstring toString(double v) {
        return fromUtf8(std::to_string(v));
    }

    jstring lowerCase(jstring value) {
        for (auto &ch: value) {
            if (ch >= u'A' && ch <= u'Z') {
                ch = static_cast<char16_t>(ch - u'A' + u'a');
            }
        }
        return value;
    }

    bool contains(const jstring &value, const jstring &needle) {
        return value.find(needle) != jstring::npos;
    }

    int_t indexOf(const jstring &value, const char16_t ch) {
        const std::size_t index = value.find(ch);
        return index == jstring::npos ? -1 : static_cast<int_t>(index);
    }

    int_t indexOf(const jstring &value, const jstring &needle) {
        const std::size_t index = value.find(needle);
        return index == jstring::npos ? -1 : static_cast<int_t>(index);
    }
}
