//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_ENTITYLIST_H
#define MCPORT_ENTITYLIST_H

#include <memory>

#include "java/String.h"
#include "java/Type.h"

class Entity;
class NBTTagCompound;
class World;

class EntityList {
public:
    static std::unique_ptr<Entity> createEntityByName(const jstring &name, World &world);

    static std::unique_ptr<Entity> createEntityFromNBT(NBTTagCompound &nbt, World &world);

    static std::unique_ptr<Entity> createEntityByID(int_t id, World &world);

    static int_t getEntityID(const Entity &entity);

    static jstring getEntityString(const Entity &entity);
};

#endif //MCPORT_ENTITYLIST_H
