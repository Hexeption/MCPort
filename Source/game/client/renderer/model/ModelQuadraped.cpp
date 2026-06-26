//
// Created by Keir Davis on 26/06/2026.
//

#include "ModelQuadraped.h"

#include <cmath>

#include "game/client/MathHelper.h"

static constexpr float PI = 3.14159265358979f;

ModelQuadraped::ModelQuadraped(const int legHeight, const float extraScale)
    : head(0, 0),
      body(28, 8),
      leg1(0, 16), leg2(0, 16),
      leg3(0, 16), leg4(0, 16) {
    head.addBox(-4.0f, -4.0f, -8.0f, 8, 8, 8, extraScale);
    head.setRotationPoint(0.0f, static_cast<float>(18 - legHeight), -6.0f);

    body.addBox(-5.0f, -10.0f, -7.0f, 10, 16, 8, extraScale);
    body.setRotationPoint(0.0f, static_cast<float>(17 - legHeight), 2.0f);

    leg1.addBox(-2.0f, 0.0f, -2.0f, 4, legHeight, 4, extraScale);
    leg1.setRotationPoint(-3.0f, static_cast<float>(24 - legHeight), 7.0f);

    leg2.addBox(-2.0f, 0.0f, -2.0f, 4, legHeight, 4, extraScale);
    leg2.setRotationPoint(3.0f, static_cast<float>(24 - legHeight), 7.0f);

    leg3.addBox(-2.0f, 0.0f, -2.0f, 4, legHeight, 4, extraScale);
    leg3.setRotationPoint(-3.0f, static_cast<float>(24 - legHeight), -5.0f);

    leg4.addBox(-2.0f, 0.0f, -2.0f, 4, legHeight, 4, extraScale);
    leg4.setRotationPoint(3.0f, static_cast<float>(24 - legHeight), -5.0f);
}

void ModelQuadraped::render(float limbSwing, float limbYaw, float ageInTicks,
                            float netHeadYaw, float headPitch, float scale) {
    setRotationAngles(limbSwing, limbYaw, ageInTicks, netHeadYaw, headPitch, scale);
    head.render(scale);
    body.render(scale);
    leg1.render(scale);
    leg2.render(scale);
    leg3.render(scale);
    leg4.render(scale);
}

void ModelQuadraped::setRotationAngles(float limbSwing, float limbYaw, float,
                                       float netHeadYaw, float headPitch, float) {
    head.rotateAngleX = -(headPitch / (180.0f / PI));
    head.rotateAngleY = netHeadYaw / (180.0f / PI);
    body.rotateAngleX = PI * 0.5f;
    leg1.rotateAngleX = MathHelper::cos(limbSwing * 0.6662f) * 1.4f * limbYaw;
    leg2.rotateAngleX = MathHelper::cos(limbSwing * 0.6662f + PI) * 1.4f * limbYaw;
    leg3.rotateAngleX = MathHelper::cos(limbSwing * 0.6662f + PI) * 1.4f * limbYaw;
    leg4.rotateAngleX = MathHelper::cos(limbSwing * 0.6662f) * 1.4f * limbYaw;
}
