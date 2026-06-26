//
// Created by Keir Davis on 25/06/2026.
//

#ifndef MCPORT_PLAYERCONTROLLERSP_H
#define MCPORT_PLAYERCONTROLLERSP_H

#include "PlayerController.h"
#include "game/world/spawner/SpawnerAnimals.h"

class PlayerControllerSP : public PlayerController {
private:
    int_t curBlockX = -1;
    int_t curBlockY = -1;
    int_t curBlockZ = -1;
    float curBlockDamage = 0.0f;
    float prevBlockDamage = 0.0f;
    float blockDestroySoundCounter = 0.0f;
    int_t blockHitWait = 0;
    SpawnerAnimals animalSpawner;

public:
    explicit PlayerControllerSP(Minecraft &minecraft);

    void flipPlayer(EntityPlayer &player) override;

    bool sendBlockRemoved(int_t x, int_t y, int_t z, int_t side) override;

    void clickBlock(int_t x, int_t y, int_t z, int_t side) override;

    void resetBlockRemoving() override;

    void sendBlockRemoving(int_t x, int_t y, int_t z, int_t side) override;

    void setPartialTime(float partialTicks) override;

    float getBlockReachDistance() override;

    void onWorldChange(World &world) override;

    void onUpdate() override;
};

#endif //MCPORT_PLAYERCONTROLLERSP_H
