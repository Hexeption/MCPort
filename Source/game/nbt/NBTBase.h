//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_NBTBASE_H
#define MCPORT_NBTBASE_H
#include "java/DataInput.h"
#include "java/DataOutput.h"
#include "java/String.h"


class NBTBase {
private:
    jstring key;

public:
    virtual ~NBTBase() = default;

    virtual void writeTagContents(DataOutput &out) = 0;

    virtual void readTagContents(DataInput &in) = 0;

    virtual byte_t getType() = 0;

    jstring getKey();

    NBTBase &setKey(jstring key);

    static NBTBase *readNamedTag(DataInput &in);

    static void writeNamedTag(NBTBase *tag, DataOutput &out);

    static NBTBase *createTagOfType(byte_t type);

    static jstring getTagName(byte_t type);

    virtual jstring toString() = 0;
};


#endif //MCPORT_NBTBASE_H
