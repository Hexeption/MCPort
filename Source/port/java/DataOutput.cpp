#include "DataOutput.h"

#include <bit>
#include <limits>
#include <stdexcept>
#include <vector>

namespace {
    template<typename T>
    void writeBigEndian(std::ostream &os, T value) {
        using U = std::make_unsigned_t<T>;
        U bits = static_cast<U>(value);

        for (int shift = static_cast<int>(sizeof(T) - 1) * 8; shift >= 0; shift -= 8) {
            os.put(static_cast<char>((bits >> shift) & 0xffU));
        }
    }

    void appendModifiedUtf8(std::vector<char> &out, const char16_t ch) {
        if (ch >= 0x0001 && ch <= 0x007f) {
            out.push_back(static_cast<char>(ch));
        } else if (ch <= 0x07ff) {
            out.push_back(static_cast<char>(0xc0 | ((ch >> 6) & 0x1f)));
            out.push_back(static_cast<char>(0x80 | (ch & 0x3f)));
        } else {
            out.push_back(static_cast<char>(0xe0 | ((ch >> 12) & 0x0f)));
            out.push_back(static_cast<char>(0x80 | ((ch >> 6) & 0x3f)));
            out.push_back(static_cast<char>(0x80 | (ch & 0x3f)));
        }
    }
}

DataOutput::DataOutput(std::ostream &os) : os(os) {
}

void DataOutput::writeBoolean(const bool b) {
    writeByte(b ? 1 : 0);
}

void DataOutput::writeByte(const int_t b) {
    os.put(static_cast<char>(b & 0xff));
}

void DataOutput::writeChar(const int_t c) {
    writeBigEndian<ushort_t>(os, static_cast<ushort_t>(c));
}

void DataOutput::writeShort(const int_t s) {
    writeBigEndian<short_t>(os, static_cast<short_t>(s));
}

void DataOutput::writeInt(const int_t i) {
    writeBigEndian<int_t>(os, i);
}

void DataOutput::writeLong(const long_t l) {
    writeBigEndian<long_t>(os, l);
}

void DataOutput::writeFloat(const float f) {
    writeBigEndian<int_t>(os, std::bit_cast<int_t>(f));
}

void DataOutput::writeDouble(const double d) {
    writeBigEndian<long_t>(os, std::bit_cast<long_t>(d));
}

void DataOutput::writeUTF(const jstring &str) {
    std::vector<char> bytes;
    bytes.reserve(str.size());

    for (const char16_t ch: str) {
        appendModifiedUtf8(bytes, ch);
    }

    if (bytes.size() > std::numeric_limits<ushort_t>::max()) {
        throw std::length_error("Encoded UTF string is too long");
    }

    writeBigEndian<ushort_t>(os, static_cast<ushort_t>(bytes.size()));
    os.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
}

void DataOutput::write(const byte_t *data, std::size_t length) {
    if (length == 0) {
        return;
    }

    os.write(reinterpret_cast<const char *>(data), static_cast<std::streamsize>(length));

    if (!os) {
        throw std::ios_base::failure("Failed to write to stream");
    }
}

void DataOutput::write(const std::vector<byte_t> &data) {
    write(data.data(), data.size());
}
