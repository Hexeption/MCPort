//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_DATAOUTPUT_H
#define MCPORT_DATAOUTPUT_H

#include <ostream>
#include <vector>

#include "String.h"
#include "Type.h"

class DataOutput {
public:
    explicit DataOutput(std::ostream &os);

    void writeBoolean(bool b);

    void writeByte(int_t b);

    void writeChar(int_t c);

    void writeShort(int_t s);

    void writeInt(int_t i);

    void writeLong(long_t l);

    void writeFloat(float f);

    void writeDouble(double d);

    void writeUTF(const jstring &str);

    void write(const byte_t *data, std::size_t length);

    void write(const std::vector<byte_t> &data);

private:
    std::ostream &os;
};

#endif //MCPORT_DATAOUTPUT_H
