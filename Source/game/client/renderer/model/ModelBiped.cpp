//
// Created by Keir Davis on 26/06/2026.
//

#include "ModelBiped.h"

#include <cmath>

#include "game/client/MathHelper.h"

ModelBiped::ModelBiped(float extraScale)
    : bipedRightArm(40, 16) {
    bipedRightArm.addBox(-3.0f, -2.0f, -2.0f, 4, 12, 4, extraScale);
    bipedRightArm.setRotationPoint(-5.0f, 2.0f, 0.0f);
}

void ModelBiped::render(float, float, float, float, float, float scale) {
    bipedRightArm.render(scale);
}

void ModelBiped::setRotationAngles(float limbSwing, float limbSwingAmount,
                                   float, float, float, float) {
    static const float PI = static_cast<float>(std::acos(-1.0));
    bipedRightArm.rotateAngleX = MathHelper::cos(limbSwing * 0.6662f + PI) * 2.0f * limbSwingAmount * 0.5f;
    bipedRightArm.rotateAngleZ = 0.0f;
    bipedRightArm.rotateAngleY = 0.0f;
}
