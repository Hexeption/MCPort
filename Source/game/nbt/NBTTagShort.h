//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_NBTTAGSHORT_H
#define MCPORT_NBTTAGSHORT_H
#include "NBTBase.h"
#include "java/DataInput.h"
#include "java/DataOutput.h"
#include "java/String.h"
#include "java/Type.h"


class NBTTagShort : public NBTBase {
public:
    short_t shortValue = 0.0;

    NBTTagShort();

    explicit NBTTagShort(short_t var1);

    void writeTagContents(DataOutput &out) override;

    void readTagContents(DataInput &in) override;

    byte_t getType() override;

    jstring toString() override;
};


#endif //MCPORT_NBTTAGSHORT_H
