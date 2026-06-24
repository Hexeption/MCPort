//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_NBTTAGCOMPOUND_H
#define MCPORT_NBTTAGCOMPOUND_H
#include "NBTBase.h"

#include <map>
#include <vector>

#include "NBTTagList.h"

class NBTTagCompound : public NBTBase {
private:
    std::map<jstring, NBTBase *> tagMap;

public:
    ~NBTTagCompound() override;

    void writeTagContents(DataOutput &out) override;

    void readTagContents(DataInput &in) override;

    byte_t getType() override;

    void setTag(const jstring &var1, NBTBase *var2);

    void setByte(const jstring &var1, byte_t var2);

    void setShort(const jstring &var1, short_t var2);

    void setInteger(const jstring &var1, int_t var2);

    void setLong(const jstring &var1, long_t var2);

    void setFloat(const jstring &var1, float var2);

    void setDouble(const jstring &var1, double var2);

    void setString(const jstring &var1, const jstring &var2);

    void setByteArray(const jstring &var1, const std::vector<byte_t> &var2);

    void setCompoundTag(const jstring &var1, NBTTagCompound *var2);

    void setBoolean(const jstring &var1, bool var2);

    bool hasKey(const jstring &var1);

    byte_t getByte(const jstring &var1);

    short_t getShort(const jstring &var1);

    int_t getInteger(const jstring &var1);

    long_t getLong(const jstring &var1);

    float getFloat(const jstring &var1);

    double getDouble(const jstring &var1);

    jstring getString(const jstring &var1);

    std::vector<byte_t> getByteArray(const jstring &var1);

    NBTTagCompound *getCompoundTag(const jstring &var1);

    NBTTagList *getTagList(const jstring &var1);

    bool getBoolean(const jstring &var1);

    jstring toString() override;
};


#endif //MCPORT_NBTTAGCOMPOUND_H
