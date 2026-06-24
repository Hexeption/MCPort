//
// Created by Keir Davis on 24/06/2026.
//

#include "NBTBase.h"

#include "NBTTagByte.h"
#include "NBTTagByteArray.h"
#include "NBTTagCompound.h"
#include "NBTTagDouble.h"
#include "NBTTagEnd.h"
#include "NBTTagFloat.h"
#include "NBTTagInt.h"
#include "NBTTagList.h"
#include "NBTTagLong.h"
#include "NBTTagShort.h"
#include "NBTTagString.h"

jstring NBTBase::getKey() {
    return this->key;
}

NBTBase &NBTBase::setKey(jstring key) {
    this->key = key;
    return *this;
}

NBTBase *NBTBase::readNamedTag(DataInput &in) {
    byte_t type = in.readByte();

    if (type == 0) {
        return new NBTTagEnd();
    }

    NBTBase *tag = createTagOfType(type);
    tag->key = in.readUTF();
    tag->readTagContents(in);

    return tag;
}

void NBTBase::writeNamedTag(NBTBase *tag, DataOutput &out) {
    out.writeByte(tag->getType());

    if (tag->getType() != 0) {
        out.writeUTF(tag->getKey());
        tag->writeTagContents(out);
    }
}

NBTBase *NBTBase::createTagOfType(byte_t type) {
    switch (type) {
        case 0: return new NBTTagEnd();
        case 1: return new NBTTagByte();
        case 2: return new NBTTagShort();
        case 3: return new NBTTagInt();
        case 4: return new NBTTagLong();
        case 5: return new NBTTagFloat();
        case 6: return new NBTTagDouble();
        case 7: return new NBTTagByteArray();
        case 8: return new NBTTagString();
        case 9: return new NBTTagList();
        case 10: return new NBTTagCompound();
        default: return nullptr;
    }
}

jstring NBTBase::getTagName(byte_t type) {
    switch (type) {
        case 0: return u"TAG_End";
        case 1: return u"TAG_Byte";
        case 2: return u"TAG_Short";
        case 3: return u"TAG_Int";
        case 4: return u"TAG_Long";
        case 5: return u"TAG_Float";
        case 6: return u"TAG_Double";
        case 7: return u"TAG_Byte_Array";
        case 8: return u"TAG_String";
        case 9: return u"TAG_List";
        case 10: return u"TAG_Compound";
        default: return u"UNKNOWN";
    }
}
