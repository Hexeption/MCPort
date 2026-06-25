//
// Created by Keir Davis on 25/06/2026.
//

#include "NextTickListEntry.h"

long_t NextTickListEntry::nextTickEntryID = 0L;

NextTickListEntry::NextTickListEntry(const int_t x, const int_t y, const int_t z, const int_t blockId)
    : xCoord(x), yCoord(y), zCoord(z), blockID(blockId), tickEntryID(nextTickEntryID++) {
}

bool NextTickListEntry::equals(const NextTickListEntry &entry) const {
    return xCoord == entry.xCoord && yCoord == entry.yCoord && zCoord == entry.zCoord && blockID == entry.blockID;
}

int_t NextTickListEntry::hashCode() const {
    return (xCoord * 128 * 1024 + zCoord * 128 + yCoord) * 256 + blockID;
}

NextTickListEntry &NextTickListEntry::setScheduledTime(const long_t time) {
    scheduledTime = time;
    return *this;
}

int_t NextTickListEntry::comparer(const NextTickListEntry &entry) const {
    if (scheduledTime < entry.scheduledTime) {
        return -1;
    }
    if (scheduledTime > entry.scheduledTime) {
        return 1;
    }
    if (tickEntryID < entry.tickEntryID) {
        return -1;
    }
    if (tickEntryID > entry.tickEntryID) {
        return 1;
    }
    return 0;
}
