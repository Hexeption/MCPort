//
// Created by Keir Davis on 24/06/2026.
//

#include "NBTTagCompound.h"

#include "NBTTagByte.h"
#include "NBTTagByteArray.h"
#include "NBTTagDouble.h"
#include "NBTTagFloat.h"
#include "NBTTagInt.h"
#include "NBTTagList.h"
#include "NBTTagLong.h"
#include "NBTTagShort.h"
#include "NBTTagString.h"

NBTTagCompound::~NBTTagCompound() {
    for (auto &var1: tagMap) {
        delete var1.second;
    }
}

void NBTTagCompound::writeTagContents(DataOutput &out) {
    for (auto &var2: tagMap) {
        NBTBase *var3 = var2.second;
        NBTBase::writeNamedTag(var3, out);
    }

    out.writeByte(0);
}

void NBTTagCompound::readTagContents(DataInput &in) {
    for (auto &var2: tagMap) {
        delete var2.second;
    }

    tagMap.clear();

    while (true) {
        NBTBase *var2 = NBTBase::readNamedTag(in);

        if (var2->getType() == 0) {
            delete var2;
            return;
        }

        tagMap[var2->getKey()] = var2;
    }
}

byte_t NBTTagCompound::getType() {
    return 10;
}

void NBTTagCompound::setTag(const jstring &var1, NBTBase *var2) {
    if (tagMap.count(var1)) {
        delete tagMap[var1];
    }

    tagMap[var1] = &var2->setKey(var1);
}

void NBTTagCompound::setByte(const jstring &var1, byte_t var2) {
    setTag(var1, new NBTTagByte(var2));
}

void NBTTagCompound::setShort(const jstring &var1, short_t var2) {
    setTag(var1, new NBTTagShort(var2));
}

void NBTTagCompound::setInteger(const jstring &var1, int_t var2) {
    setTag(var1, new NBTTagInt(var2));
}

void NBTTagCompound::setLong(const jstring &var1, long_t var2) {
    setTag(var1, new NBTTagLong(var2));
}

void NBTTagCompound::setFloat(const jstring &var1, float var2) {
    setTag(var1, new NBTTagFloat(var2));
}

void NBTTagCompound::setDouble(const jstring &var1, double var2) {
    setTag(var1, new NBTTagDouble(var2));
}

void NBTTagCompound::setString(const jstring &var1, const jstring &var2) {
    setTag(var1, new NBTTagString(var2));
}

void NBTTagCompound::setByteArray(const jstring &var1, const std::vector<byte_t> &var2) {
    setTag(var1, new NBTTagByteArray(var2));
}

void NBTTagCompound::setCompoundTag(const jstring &var1, NBTTagCompound *var2) {
    setTag(var1, var2);
}

void NBTTagCompound::setBoolean(const jstring &var1, bool var2) {
    setByte(var1, static_cast<byte_t>(var2 ? 1 : 0));
}

bool NBTTagCompound::hasKey(const jstring &var1) {
    return tagMap.count(var1) != 0;
}

byte_t NBTTagCompound::getByte(const jstring &var1) {
    return !hasKey(var1) ? 0 : static_cast<NBTTagByte *>(tagMap[var1])->byteValue;
}

short_t NBTTagCompound::getShort(const jstring &var1) {
    return !hasKey(var1) ? 0 : static_cast<NBTTagShort *>(tagMap[var1])->shortValue;
}

int_t NBTTagCompound::getInteger(const jstring &var1) {
    return !hasKey(var1) ? 0 : static_cast<NBTTagInt *>(tagMap[var1])->intValue;
}

long_t NBTTagCompound::getLong(const jstring &var1) {
    return !hasKey(var1) ? 0L : static_cast<NBTTagLong *>(tagMap[var1])->longValue;
}

float NBTTagCompound::getFloat(const jstring &var1) {
    return !hasKey(var1) ? 0.0F : static_cast<NBTTagFloat *>(tagMap[var1])->floatValue;
}

double NBTTagCompound::getDouble(const jstring &var1) {
    return !hasKey(var1) ? 0.0 : static_cast<NBTTagDouble *>(tagMap[var1])->doubleValue;
}

jstring NBTTagCompound::getString(const jstring &var1) {
    return !hasKey(var1) ? u"" : static_cast<NBTTagString *>(tagMap[var1])->stringValue;
}

std::vector<byte_t> NBTTagCompound::getByteArray(const jstring &var1) {
    return !hasKey(var1) ? std::vector<byte_t>() : static_cast<NBTTagByteArray *>(tagMap[var1])->byteArray;
}

NBTTagCompound *NBTTagCompound::getCompoundTag(const jstring &var1) {
    return !hasKey(var1) ? new NBTTagCompound() : static_cast<NBTTagCompound *>(tagMap[var1]);
}

NBTTagList *NBTTagCompound::getTagList(const jstring &var1) {
    return !hasKey(var1) ? new NBTTagList() : static_cast<NBTTagList *>(tagMap[var1]);
}

bool NBTTagCompound::getBoolean(const jstring &var1) {
    return getByte(var1) != 0;
}

jstring NBTTagCompound::toString() {
    return String::toString(static_cast<int_t>(tagMap.size())) + u" entries";
}
