//
// Created by Keir Davis on 26/06/2026.
//

#include "EntityList.h"

#include <iostream>
#include <typeinfo>

#include "game/entity/Entity.h"
#include "game/entity/EntityCow.h"
#include "game/entity/EntityFallingSand.h"
#include "game/entity/EntityItem.h"
#include "game/entity/EntityLiving.h"
#include "game/nbt/NBTTagCompound.h"
#include "game/world/World.h"

std::unique_ptr<Entity> EntityList::createEntityByName(const jstring &name, World &world) {
    if (name == u"Item") {
        return std::make_unique<EntityItem>(world);
    }
    if (name == u"Mob") {
        return std::make_unique<EntityLiving>(world);
    }
    if (name == u"Cow") {
        return std::make_unique<EntityCow>(world);
    }
    if (name == u"FallingSand") {
        return std::make_unique<EntityFallingSand>(world);
    }
    return nullptr;
}

std::unique_ptr<Entity> EntityList::createEntityFromNBT(NBTTagCompound &nbt, World &world) {
    std::unique_ptr<Entity> entity = createEntityByName(nbt.getString(u"id"), world);
    if (entity != nullptr) {
        entity->readFromNBT(nbt);
    } else {
        std::cerr << "Skipping Entity with id " << String::toUtf8(nbt.getString(u"id")) << '\n';
    }
    return entity;
}

std::unique_ptr<Entity> EntityList::createEntityByID(const int_t id, World &world) {
    if (id == 1) {
        return std::make_unique<EntityItem>(world);
    }
    if (id == 48) {
        return std::make_unique<EntityLiving>(world);
    }
    if (id == 91) {
        return std::make_unique<EntityCow>(world);
    }
    if (id == 21) {
        return std::make_unique<EntityFallingSand>(world);
    }

    std::cerr << "Skipping Entity with id " << id << '\n';
    return nullptr;
}

int_t EntityList::getEntityID(const Entity &entity) {
    if (typeid(entity) == typeid(EntityItem)) {
        return 1;
    }
    if (typeid(entity) == typeid(EntityLiving)) {
        return 48;
    }
    if (typeid(entity) == typeid(EntityCow)) {
        return 91;
    }
    if (typeid(entity) == typeid(EntityFallingSand)) {
        return 21;
    }
    return 0;
}

jstring EntityList::getEntityString(const Entity &entity) {
    if (typeid(entity) == typeid(EntityItem)) {
        return u"Item";
    }
    if (typeid(entity) == typeid(EntityLiving)) {
        return u"Mob";
    }
    if (typeid(entity) == typeid(EntityCow)) {
        return u"Cow";
    }
    if (typeid(entity) == typeid(EntityFallingSand)) {
        return u"FallingSand";
    }
    return u"";
}
