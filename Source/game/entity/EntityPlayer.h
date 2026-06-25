//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_ENTITYPLAYER_H
#define MCPORT_ENTITYPLAYER_H

#include "EntityLiving.h"
#include "java/Type.h"

class Block;

class EntityPlayer : public EntityLiving {
public:
    int_t score = 0;
    float prevCameraYaw = 0.0f;
    float cameraYaw = 0.0f;
    bool isSwinging = false;
    int_t swingProgressInt = 0;

    explicit EntityPlayer(World &world);

    void preparePlayerToSpawn() override;

    void onLivingUpdate() override;

    float getCurrentPlayerStrVsBlock(const Block &block);

    bool canHarvestBlock(const Block &block);

    void swingItem();
};

#endif //MCPORT_ENTITYPLAYER_H
