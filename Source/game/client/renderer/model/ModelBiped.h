//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_MODELBIPED_H
#define MCPORT_MODELBIPED_H

#include "ModelBase.h"
#include "ModelRenderer.h"

class ModelBiped : public ModelBase {
public:
    ModelRenderer bipedRightArm;

    explicit ModelBiped(float extraScale = 0.0f);

    void render(float limbSwing, float limbYaw, float ageInTicks,
                float netHeadYaw, float headPitch, float scale) override;

    void setRotationAngles(float limbSwing, float limbYaw, float ageInTicks,
                           float netHeadYaw, float headPitch, float scale) override;
};

#endif //MCPORT_MODELBIPED_H
