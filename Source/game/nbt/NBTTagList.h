//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_NBTTAGLIST_H
#define MCPORT_NBTTAGLIST_H

#include <vector>

#include "NBTBase.h"


class NBTTagList : public NBTBase {
private:
    std::vector<NBTBase *> tagList;
    byte_t tagType = 0;

public:
    ~NBTTagList() override;

    void writeTagContents(DataOutput &var1) override;

    void readTagContents(DataInput &var1) override;

    byte_t getType() override;

    jstring toString() override;

    void setTag(NBTBase *var1);

    NBTBase *entities(int_t var1);

    NBTBase *tagAt(int_t var1);

    int_t tagCount();
};


#endif //MCPORT_NBTTAGLIST_H
