//
// Created by Keir Davis on 24/06/2026.
//

#include "NBTTagLong.h"

NBTTagLong::NBTTagLong() = default;

NBTTagLong::NBTTagLong(long_t var1) : longValue(var1) {
}

void NBTTagLong::writeTagContents(DataOutput &out) {
    out.writeLong(longValue);
}

void NBTTagLong::readTagContents(DataInput &in) {
    longValue = in.readLong();
}

byte_t NBTTagLong::getType() {
    return 4;
}

jstring NBTTagLong::toString() {
    return String::toString(longValue);
}
