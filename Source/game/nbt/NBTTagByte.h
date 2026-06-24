//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_NBTTAGBYTE_H
#define MCPORT_NBTTAGBYTE_H
#include "NBTBase.h"


class NBTTagByte : public NBTBase {
public:
    byte_t byteValue = 0;

    NBTTagByte() = default;

    explicit NBTTagByte(byte_t value);

    void writeTagContents(DataOutput &out) override;

    void readTagContents(DataInput &in) override;

    byte_t getType() override;

    jstring toString() override;
};


#endif //MCPORT_NBTTAGBYTE_H
