//
// Created by Keir Davis on 23/06/2026.
//

#include "ZombieModel.h"

#include <cmath>

#include "java/Math.h"

ZombieModel::ZombieModel()
    : head(0, 0),
      body(16, 16),
      arm0(40, 16),
      arm1(40, 16),
      leg0(0, 16),
      leg1(0, 16) {
    this->head.addBox(-4.0F, -8.0F, -4.0F, 8, 8, 8);
    this->body.addBox(-4.0F, 0.0F, -2.0F, 8, 12, 4);
    this->arm0.addBox(-3.0F, -2.0F, -2.0F, 4, 12, 4);
    this->arm0.setPos(-5.0F, 2.0F, 0.0F);
    this->arm1.addBox(-1.0F, -2.0F, -2.0F, 4, 12, 4);
    this->arm1.setPos(5.0F, 2.0F, 0.0F);
    this->leg0.addBox(-2.0F, 0.0F, -2.0F, 4, 12, 4);
    this->leg0.setPos(-2.0F, 12.0F, 0.0F);
    this->leg1.addBox(-2.0F, 0.0F, -2.0F, 4, 12, 4);
    this->leg1.setPos(2.0F, 12.0F, 0.0F);
}

void ZombieModel::render(float time) {
    this->head.yRot = static_cast<float>(std::sin(static_cast<double>(time) * 0.83)) * 1.0F;
    this->head.xRot = static_cast<float>(std::sin(static_cast<double>(time))) * 0.8F;
    this->arm0.xRot = static_cast<float>(std::sin(static_cast<double>(time) * 0.6662 + Math::PI)) * 2.0F;
    this->arm0.zRot = static_cast<float>(std::sin(static_cast<double>(time) * 0.2312) + 1.0) * 1.0F;
    this->arm1.xRot = static_cast<float>(std::sin(static_cast<double>(time) * 0.6662)) * 2.0F;
    this->arm1.zRot = static_cast<float>(std::sin(static_cast<double>(time) * 0.2812) - 1.0) * 1.0F;
    this->leg0.xRot = static_cast<float>(std::sin(static_cast<double>(time) * 0.6662)) * 1.4F;
    this->leg1.xRot = static_cast<float>(std::sin(static_cast<double>(time) * 0.6662 + Math::PI)) * 1.4F;
    this->head.render();
    this->body.render();
    this->arm0.render();
    this->arm1.render();
    this->leg0.render();
    this->leg1.render();
}
