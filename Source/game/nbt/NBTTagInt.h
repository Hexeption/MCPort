//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_NBTTAGINT_H
#define MCPORT_NBTTAGINT_H
#include "NBTBase.h"
#include "java/DataInput.h"
#include "java/DataOutput.h"
#include "java/String.h"
#include "java/Type.h"

class NBTTagInt : public NBTBase {
public:
    int_t intValue = 0.0;

    NBTTagInt();

    explicit NBTTagInt(int_t var1);

    void writeTagContents(DataOutput &out) override;

    void readTagContents(DataInput &in) override;

    byte_t getType() override;

    jstring toString() override;
};


#endif //MCPORT_NBTTAGINT_H
