//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_MODELCOW_H
#define MCPORT_MODELCOW_H

#include "ModelQuadraped.h"

class ModelCow : public ModelQuadraped {
public:
    ModelRenderer udders;
    ModelRenderer horn1;
    ModelRenderer horn2;

    ModelCow();

    void render(float limbSwing, float limbYaw, float ageInTicks,
                float netHeadYaw, float headPitch, float scale) override;

    void setRotationAngles(float limbSwing, float limbYaw, float ageInTicks,
                           float netHeadYaw, float headPitch, float scale) override;
};

#endif //MCPORT_MODELCOW_H
