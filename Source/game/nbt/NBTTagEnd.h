//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_NBTTAGEND_H
#define MCPORT_NBTTAGEND_H
#include "NBTBase.h"
#include "java/DataInput.h"
#include "java/DataOutput.h"
#include "java/String.h"
#include "java/Type.h"


class NBTTagEnd : public NBTBase {
public:
    NBTTagEnd() = default;

    void writeTagContents(DataOutput &out) override;

    void readTagContents(DataInput &in) override;

    byte_t getType() override;

    jstring toString() override;
};


#endif //MCPORT_NBTTAGEND_H
