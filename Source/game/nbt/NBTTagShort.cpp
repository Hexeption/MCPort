//
// Created by Keir Davis on 24/06/2026.
//

#include "NBTTagShort.h"

NBTTagShort::NBTTagShort() = default;

NBTTagShort::NBTTagShort(short_t var1) : shortValue(var1) {
}

void NBTTagShort::writeTagContents(DataOutput &out) {
    out.writeShort(shortValue);
}

void NBTTagShort::readTagContents(DataInput &in) {
    shortValue = in.readShort();
}

byte_t NBTTagShort::getType() {
    return 2;
}

jstring NBTTagShort::toString() {
    return String::toString(shortValue);
}
