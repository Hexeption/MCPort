//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_DATAINPUT_H
#define MCPORT_DATAINPUT_H

#include <istream>
#include <vector>

#include "String.h"
#include "Type.h"

class DataInput {
public:
    explicit DataInput(std::istream &is);

    bool readBoolean();

    byte_t readByte();

    char_t readChar();

    short_t readShort();

    int_t readInt();

    long_t readLong();

    float readFloat();

    double readDouble();

    jstring readUTF();

    void readFully(byte_t *data, std::size_t length);

    void readFully(std::vector<byte_t> &data);

    std::vector<char> readAllBytes();

private:
    std::istream &is;
};

#endif //MCPORT_DATAINPUT_H
