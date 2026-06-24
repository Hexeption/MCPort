//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_NBTTAGDOUBLE_H
#define MCPORT_NBTTAGDOUBLE_H
#include "NBTBase.h"
#include "java/DataInput.h"
#include "java/DataOutput.h"
#include "java/String.h"
#include "java/Type.h"


class NBTTagDouble : public NBTBase {
public:
    double doubleValue = 0.0;

    NBTTagDouble();

    explicit NBTTagDouble(double var1);

    void writeTagContents(DataOutput &out) override;

    void readTagContents(DataInput &in) override;

    byte_t getType() override;

    jstring toString() override;
};


#endif //MCPORT_NBTTAGDOUBLE_H
