//
// Created by Keir Davis on 25/06/2026.
//

#include "EntityPlayerSP.h"

#include "EntityLiving.h"
#include "MovementInputFromOptions.h"
#include "game/client/Minecraft.h"
#include "game/client/gui/GuiCrafting.h"
#include "game/item/ItemStack.h"
#include "lwjgl/Keyboard.h"

EntityPlayerSP::EntityPlayerSP(World &world, GameSettings &gameSettings) : EntityPlayer(world),
                                                                           movementInput(
                                                                               std::make_unique<
                                                                                   MovementInputFromOptions>(
                                                                                   gameSettings)) {
}

void EntityPlayerSP::onLivingUpdate() {
    if (noclip) {
        const float speed = 0.3f;

        movementInput->updatePlayerMoveState(*this);

        motionX = 0.0;
        motionY = 0.0;
        motionZ = 0.0;

        moveFlying(movementInput->moveStrafe,
                   movementInput->moveForward,
                   speed);

        if (lwjgl::Keyboard::isKeyDown(lwjgl::Keyboard::KEY_SPACE))
            motionY += speed;

        if (lwjgl::Keyboard::isKeyDown(lwjgl::Keyboard::KEY_LSHIFT))
            motionY -= speed;

        posX += motionX;
        posY += motionY;
        posZ += motionZ;

        return;
    }
    movementInput->updatePlayerMoveState(*this);
    moveStrafing = movementInput->moveStrafe;
    moveForward = movementInput->moveForward;
    isJumping = movementInput->jump;
    EntityPlayer::onLivingUpdate();
}

void EntityPlayerSP::resetPlayerKeyState() {
    movementInput->resetKeyState();
}

void EntityPlayerSP::handleKeyPress(const int_t key, const bool pressed) {
    movementInput->checkKeyForMovementInput(key, pressed);
}

void EntityPlayerSP::displayWorkbenchGUI() {
    if (mc != nullptr) {
        mc->displayGuiScreen(std::make_shared<GuiCrafting>(inventory));
    }
}

void EntityPlayerSP::attackEntity(Entity &entity) {
    const int_t damage = inventory.getDamageVsEntity(&entity);
    if (damage <= 0) {
        return;
    }

    entity.attackEntityFrom(this, damage);

    if (auto *living = dynamic_cast<EntityLiving *>(&entity)) {
        if (ItemStack *stack = getCurrentEquippedItem()) {
            stack->hitEntity(*living);
            if (stack->stackSize <= 0) {
                destroyCurrentEquippedItem();
            }
        }
    }
}
