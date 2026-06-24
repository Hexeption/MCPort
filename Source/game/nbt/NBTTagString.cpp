//
// Created by Keir Davis on 24/06/2026.
//

#include "NBTTagString.h"
#include <stdexcept>

NBTTagString::NBTTagString() = default;

NBTTagString::NBTTagString(const jstring &var1)
    : stringValue(var1) {
}

void NBTTagString::writeTagContents(DataOutput &var1) {
    var1.writeUTF(stringValue);
}

void NBTTagString::readTagContents(DataInput &var1) {
    stringValue = var1.readUTF();
}

byte_t NBTTagString::getType() {
    return 8;
}

jstring NBTTagString::toString() {
    return stringValue;
}
