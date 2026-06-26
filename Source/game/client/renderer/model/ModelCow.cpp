//
// Created by Keir Davis on 26/06/2026.
//

#include "ModelCow.h"

static constexpr float PI = 3.14159265358979f;

ModelCow::ModelCow()
    : ModelQuadraped(12, 0.0f),
      udders(52, 0),
      horn1(22, 0),
      horn2(22, 0) {
    head = ModelRenderer(0, 0);
    head.addBox(-4.0f, -4.0f, -6.0f, 8, 8, 6, 0.0f);
    head.setRotationPoint(0.0f, 4.0f, -8.0f);

    horn1.addBox(-5.0f, -5.0f, -4.0f, 1, 3, 1, 0.0f);
    horn1.setRotationPoint(0.0f, 3.0f, -7.0f);

    horn2.addBox(4.0f, -5.0f, -4.0f, 1, 3, 1, 0.0f);
    horn2.setRotationPoint(0.0f, 3.0f, -7.0f);

    udders.addBox(-2.0f, -3.0f, 0.0f, 4, 6, 2, 0.0f);
    udders.setRotationPoint(0.0f, 14.0f, 6.0f);
    udders.rotateAngleX = PI * 0.5f;

    body = ModelRenderer(18, 4);
    body.addBox(-6.0f, -10.0f, -7.0f, 12, 18, 10, 0.0f);
    body.setRotationPoint(0.0f, 5.0f, 2.0f);

    --leg1.rotationPointX;
    ++leg2.rotationPointX;
    --leg3.rotationPointX;
    ++leg4.rotationPointX;
    --leg3.rotationPointZ;
    --leg4.rotationPointZ;
}

void ModelCow::render(float limbSwing, float limbYaw, float ageInTicks,
                      float netHeadYaw, float headPitch, float scale) {
    ModelQuadraped::render(limbSwing, limbYaw, ageInTicks, netHeadYaw, headPitch, scale);
    horn1.render(scale);
    horn2.render(scale);
    udders.render(scale);
}

void ModelCow::setRotationAngles(float limbSwing, float limbYaw, float ageInTicks,
                                  float netHeadYaw, float headPitch, float scale) {
    ModelQuadraped::setRotationAngles(limbSwing, limbYaw, ageInTicks, netHeadYaw, headPitch, scale);
    horn1.rotateAngleY = head.rotateAngleY;
    horn1.rotateAngleX = head.rotateAngleX;
    horn2.rotateAngleY = head.rotateAngleY;
    horn2.rotateAngleX = head.rotateAngleX;
}
