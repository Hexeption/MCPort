//
// Created by Keir Davis on 24/06/2026.
//

#include "NBTTagByte.h"

NBTTagByte::NBTTagByte(byte_t value) : byteValue(value) {
}

void NBTTagByte::writeTagContents(DataOutput &out) {
    out.writeByte(byteValue);
}

void NBTTagByte::readTagContents(DataInput &in) {
    byteValue = in.readByte();
}

byte_t NBTTagByte::getType() {
    return 1;
}

jstring NBTTagByte::toString() {
    return String::toString(byteValue);
}
