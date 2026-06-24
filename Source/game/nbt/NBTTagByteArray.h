//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_NBTTAGBYTEARRAY_H
#define MCPORT_NBTTAGBYTEARRAY_H

#include <vector>

#include "NBTBase.h"
#include "java/DataInput.h"
#include "java/DataOutput.h"
#include "java/String.h"

class NBTTagByteArray : public NBTBase {
public :
    std::vector<byte_t> byteArray;

    NBTTagByteArray();

    explicit NBTTagByteArray(const std::vector<byte_t> &value);

    void writeTagContents(DataOutput &out) override;

    void readTagContents(DataInput &in) override;

    byte_t getType() override;

    jstring toString() override;
};


#endif //MCPORT_NBTTAGBYTEARRAY_H
