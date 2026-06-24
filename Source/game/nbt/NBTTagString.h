//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_NBTTAGSTRING_H
#define MCPORT_NBTTAGSTRING_H
#include "NBTBase.h"
#include "java/DataInput.h"
#include "java/DataOutput.h"
#include "java/String.h"
#include "java/Type.h"


class NBTTagString : public NBTBase {
public:
    jstring stringValue;

    NBTTagString();

    explicit NBTTagString(const jstring &var1);

    void writeTagContents(DataOutput &var1) override;

    void readTagContents(DataInput &var1) override;

    byte_t getType() override;

    jstring toString() override;
};


#endif //MCPORT_NBTTAGSTRING_H
