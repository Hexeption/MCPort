//
// Created by Keir Davis on 23/06/2026.
//

#include "Player.h"

#include "lwjgl/Keyboard.h"

Player::Player(Level &level)
    : Entity(level) {
    this->heightOffset = 1.62F;
}

void Player::tick() {
    this->xo = this->x;
    this->yo = this->y;
    this->zo = this->z;
    float xa = 0.0F;
    float ya = 0.0F;
    if (lwjgl::Keyboard::isKeyDown(19)) {
        this->resetPos();
    }

    if (lwjgl::Keyboard::isKeyDown(200) || lwjgl::Keyboard::isKeyDown(17)) {
        --ya;
    }

    if (lwjgl::Keyboard::isKeyDown(208) || lwjgl::Keyboard::isKeyDown(31)) {
        ++ya;
    }

    if (lwjgl::Keyboard::isKeyDown(203) || lwjgl::Keyboard::isKeyDown(30)) {
        --xa;
    }

    if (lwjgl::Keyboard::isKeyDown(205) || lwjgl::Keyboard::isKeyDown(32)) {
        ++xa;
    }

    if ((lwjgl::Keyboard::isKeyDown(57) || lwjgl::Keyboard::isKeyDown(219)) && this->onGround) {
        this->yd = 0.5F;
    }

    this->moveRelative(xa, ya, this->onGround ? 0.1F : 0.02F);
    this->yd = static_cast<float>(static_cast<double>(this->yd) - 0.08);
    this->move(this->xd, this->yd, this->zd);
    this->xd *= 0.91F;
    this->yd *= 0.98F;
    this->zd *= 0.91F;
    if (this->onGround) {
        this->xd *= 0.7F;
        this->zd *= 0.7F;
    }
}
