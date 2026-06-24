//
// Created by Keir Davis on 24/06/2026.
//

#include "NBTTagInt.h"

NBTTagInt::NBTTagInt() = default;

NBTTagInt::NBTTagInt(int_t var1) : intValue(var1) {
}

void NBTTagInt::writeTagContents(DataOutput &out) {
    out.writeInt(intValue);
}

void NBTTagInt::readTagContents(DataInput &in) {
    intValue = in.readInt();
}

byte_t NBTTagInt::getType() {
    return 3;
}

jstring NBTTagInt::toString() {
    return String::toString(intValue);
}
