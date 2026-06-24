//
// Created by Keir Davis on 24/06/2026.
//

#include "NBTTagByteArray.h"

NBTTagByteArray::NBTTagByteArray() = default;

NBTTagByteArray::NBTTagByteArray(const std::vector<byte_t> &value) : byteArray(value) {
}

void NBTTagByteArray::writeTagContents(DataOutput &out) {
    out.writeInt(static_cast<int_t>(byteArray.size()));
    out.write(byteArray);
}

void NBTTagByteArray::readTagContents(DataInput &in) {
    int_t length = in.readInt();
    byteArray.resize(length);
    in.readFully(byteArray);
}

byte_t NBTTagByteArray::getType() {
    return 7;
}

jstring NBTTagByteArray::toString() {
    return u"[" + String::toString(static_cast<int_t>(byteArray.size())) + u" bytes]";
}
