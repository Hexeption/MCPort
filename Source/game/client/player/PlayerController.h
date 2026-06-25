//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_PLAYERCONTROLLER_H
#define MCPORT_PLAYERCONTROLLER_H

#include "java/Type.h"

class EntityPlayer;
class Minecraft;
class World;

class PlayerController {
protected:
    Minecraft &mc;

public:
    bool isInTestMode = false;

    explicit PlayerController(Minecraft &minecraft);

    virtual ~PlayerController() = default;

    virtual void onWorldChange(World &world);

    virtual void clickBlock(int_t x, int_t y, int_t z, int_t side);

    virtual bool sendBlockRemoved(int_t x, int_t y, int_t z, int_t side);

    virtual void sendBlockRemoving(int_t x, int_t y, int_t z, int_t side);

    virtual void resetBlockRemoving();

    virtual void setPartialTime(float partialTicks);

    virtual float getBlockReachDistance();

    virtual void flipPlayer(EntityPlayer &player);

    virtual void onUpdate();

    virtual bool shouldDrawHUD();

    virtual void onRespawn(EntityPlayer &player);
};

#endif //MCPORT_PLAYERCONTROLLER_H
