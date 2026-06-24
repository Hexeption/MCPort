//
// Created by Keir Davis on 24/06/2026.
//

#include "NBTTagEnd.h"

void NBTTagEnd::readTagContents(DataInput &var1) {
}

void NBTTagEnd::writeTagContents(DataOutput &var1) {
}

byte_t NBTTagEnd::getType() {
    return 0;
}

jstring NBTTagEnd::toString() {
    return u"END";
}
