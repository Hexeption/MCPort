#include "DataInput.h"

#include <bit>
#include <iterator>
#include <stdexcept>

namespace {
    template<typename T>
    T readBigEndian(std::istream &is) {
        using U = std::make_unsigned_t<T>;
        U value = 0;

        for (std::size_t i = 0; i < sizeof(T); ++i) {
            const int ch = is.get();
            if (ch == std::char_traits<char>::eof()) {
                throw std::ios_base::failure("Unexpected end of stream");
            }
            value = static_cast<U>((value << 8U) | static_cast<U>(ch & 0xff));
        }

        return static_cast<T>(value);
    }

    unsigned char readContinuation(const std::vector<char> &bytes, std::size_t &index) {
        if (index >= bytes.size()) {
            throw std::range_error("Malformed modified UTF-8 sequence");
        }

        const auto ch = static_cast<unsigned char>(bytes[index++]);
        if ((ch & 0xc0) != 0x80) {
            throw std::range_error("Malformed modified UTF-8 sequence");
        }

        return ch;
    }
}

DataInput::DataInput(std::istream &is) : is(is) {
}

bool DataInput::readBoolean() {
    return readByte() != 0;
}

byte_t DataInput::readByte() {
    return readBigEndian<byte_t>(is);
}

char_t DataInput::readChar() {
    return static_cast<char_t>(readBigEndian<ushort_t>(is));
}

short_t DataInput::readShort() {
    return readBigEndian<short_t>(is);
}

int_t DataInput::readInt() {
    return readBigEndian<int_t>(is);
}

long_t DataInput::readLong() {
    return readBigEndian<long_t>(is);
}

float DataInput::readFloat() {
    return std::bit_cast<float>(readBigEndian<int_t>(is));
}

double DataInput::readDouble() {
    return std::bit_cast<double>(readBigEndian<long_t>(is));
}

jstring DataInput::readUTF() {
    const ushort_t length = readBigEndian<ushort_t>(is);
    std::vector<char> bytes(length);

    if (!bytes.empty()) {
        is.read(bytes.data(), static_cast<std::streamsize>(bytes.size()));
        if (is.gcount() != static_cast<std::streamsize>(bytes.size())) {
            throw std::ios_base::failure("Unexpected end of stream");
        }
    }

    jstring result;
    result.reserve(bytes.size());

    for (std::size_t i = 0; i < bytes.size();) {
        const auto ch = static_cast<unsigned char>(bytes[i++]);

        if (ch <= 0x7f) {
            if (ch == 0) {
                throw std::range_error("Malformed modified UTF-8 sequence");
            }
            result.push_back(static_cast<char16_t>(ch));
        } else if ((ch & 0xe0) == 0xc0) {
            const unsigned char ch2 = readContinuation(bytes, i);
            const char16_t decoded = static_cast<char16_t>(((ch & 0x1f) << 6) | (ch2 & 0x3f));
            if (decoded < 0x80 && decoded != 0) {
                throw std::range_error("Malformed modified UTF-8 sequence");
            }
            result.push_back(decoded);
        } else if ((ch & 0xf0) == 0xe0) {
            const unsigned char ch2 = readContinuation(bytes, i);
            const unsigned char ch3 = readContinuation(bytes, i);
            const char16_t decoded = static_cast<char16_t>(((ch & 0x0f) << 12)
                                                           | ((ch2 & 0x3f) << 6)
                                                           | (ch3 & 0x3f));
            if (decoded < 0x800) {
                throw std::range_error("Malformed modified UTF-8 sequence");
            }
            result.push_back(decoded);
        } else {
            throw std::range_error("Malformed modified UTF-8 sequence");
        }
    }

    return result;
}

std::vector<char> DataInput::readAllBytes() {
    return std::vector<char>(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>());
}

void DataInput::readFully(byte_t *data, std::size_t length) {
    if (length == 0) {
        return;
    }

    is.read(reinterpret_cast<char *>(data), static_cast<std::streamsize>(length));

    if (is.gcount() != static_cast<std::streamsize>(length)) {
        throw std::ios_base::failure("Unexpected end of stream");
    }
}

void DataInput::readFully(std::vector<byte_t> &data) {
    readFully(data.data(), data.size());
}
