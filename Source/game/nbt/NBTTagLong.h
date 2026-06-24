//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_NBTTAGLONG_H
#define MCPORT_NBTTAGLONG_H
#include "NBTBase.h"
#include "java/DataInput.h"
#include "java/DataOutput.h"
#include "java/String.h"
#include "java/Type.h"


class NBTTagLong : public NBTBase {
public:
    long_t longValue = 0.0;

    NBTTagLong();

    explicit NBTTagLong(long_t var1);

    void writeTagContents(DataOutput &out) override;

    void readTagContents(DataInput &in) override;

    byte_t getType() override;

    jstring toString() override;
};


#endif //MCPORT_NBTTAGLONG_H
