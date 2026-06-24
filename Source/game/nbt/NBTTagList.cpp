//
// Created by Keir Davis on 24/06/2026.
//

#include "NBTTagList.h"

NBTTagList::~NBTTagList() {
    for (NBTBase *var1: tagList) {
        delete var1;
    }
}

void NBTTagList::writeTagContents(DataOutput &out) {
    if (!tagList.empty()) {
        tagType = tagList[0]->getType();
    } else {
        tagType = 1;
    }

    out.writeByte(tagType);
    out.writeInt(static_cast<int_t>(tagList.size()));

    for (int_t var2 = 0; var2 < static_cast<int_t>(tagList.size()); ++var2) {
        tagList[var2]->writeTagContents(out);
    }
}

void NBTTagList::readTagContents(DataInput &in) {
    tagType = in.readByte();
    int_t var2 = in.readInt();

    for (NBTBase *var3: tagList) {
        delete var3;
    }

    tagList.clear();

    for (int_t var3 = 0; var3 < var2; ++var3) {
        NBTBase *var4 = NBTBase::createTagOfType(tagType);
        var4->readTagContents(in);
        tagList.push_back(var4);
    }
}

byte_t NBTTagList::getType() {
    return 9;
}

jstring NBTTagList::toString() {
    return String::toString(static_cast<int_t>(tagList.size())) + u" entries of type " + NBTBase::getTagName(tagType);
}

void NBTTagList::setTag(NBTBase *var1) {
    tagType = var1->getType();
    tagList.push_back(var1);
}

NBTBase *NBTTagList::entities(int_t var1) {
    return tagList[var1];
}

NBTBase *NBTTagList::tagAt(int_t var1) {
    return tagList[var1];
}

int_t NBTTagList::tagCount() {
    return static_cast<int_t>(tagList.size());
}
