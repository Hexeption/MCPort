//
// Created by Keir Davis on 24/06/2026.
//

#include "NBTTagFloat.h"

NBTTagFloat::NBTTagFloat() = default;

NBTTagFloat::NBTTagFloat(double var1) : floatValue(var1) {
}

void NBTTagFloat::writeTagContents(DataOutput &out) {
    out.writeFloat(floatValue);
}

void NBTTagFloat::readTagContents(DataInput &in) {
    floatValue = in.readFloat();
}

byte_t NBTTagFloat::getType() {
    return 5;
}

jstring NBTTagFloat::toString() {
    return String::toString(floatValue);
}
