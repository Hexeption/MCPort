//
// Created by Keir Davis on 24/06/2026.
//

#include "NBTTagDouble.h"

NBTTagDouble::NBTTagDouble() = default;

NBTTagDouble::NBTTagDouble(double var1)
    : doubleValue(var1) {
}

void NBTTagDouble::writeTagContents(DataOutput &out) {
    out.writeDouble(doubleValue);
}

void NBTTagDouble::readTagContents(DataInput &in) {
    doubleValue = in.readDouble();
}

byte_t NBTTagDouble::getType() {
    return 6;
}

jstring NBTTagDouble::toString() {
    return String::toString(doubleValue);
}
