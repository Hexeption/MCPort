//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_NEXTTICKLISTENTRY_H
#define MCPORT_NEXTTICKLISTENTRY_H

#include "java/Type.h"

class NextTickListEntry {
public:
    int_t xCoord;
    int_t yCoord;
    int_t zCoord;
    int_t blockID;
    long_t scheduledTime = 0;
    long_t tickEntryID;

    NextTickListEntry(int_t x, int_t y, int_t z, int_t blockId);

    bool equals(const NextTickListEntry &entry) const;

    int_t hashCode() const;

    NextTickListEntry &setScheduledTime(long_t time);

    int_t comparer(const NextTickListEntry &entry) const;

private:
    static long_t nextTickEntryID;
};

#endif //MCPORT_NEXTTICKLISTENTRY_H
