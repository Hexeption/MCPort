//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_MODELQUADRAPED_H
#define MCPORT_MODELQUADRAPED_H

#include "ModelBase.h"
#include "ModelRenderer.h"

class ModelQuadraped : public ModelBase {
public:
    ModelRenderer head;
    ModelRenderer body;
    ModelRenderer leg1;
    ModelRenderer leg2;
    ModelRenderer leg3;
    ModelRenderer leg4;

    ModelQuadraped(int legHeight, float extraScale);

    void render(float limbSwing, float limbYaw, float ageInTicks,
                float netHeadYaw, float headPitch, float scale) override;

    void setRotationAngles(float limbSwing, float limbYaw, float ageInTicks,
                           float netHeadYaw, float headPitch, float scale) override;
};

#endif //MCPORT_MODELQUADRAPED_H
