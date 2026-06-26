//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_MODELBASE_H
#define MCPORT_MODELBASE_H

class ModelBase {
public:
    float swingProgress = 0.0f;
    bool isRiding = false;

    virtual ~ModelBase() = default;

    virtual void render(float limbSwing, float limbYaw, float ageInTicks,
                        float netHeadYaw, float headPitch, float scale);

    virtual void setRotationAngles(float limbSwing, float limbYaw, float ageInTicks,
                                   float netHeadYaw, float headPitch, float scale);
};

#endif //MCPORT_MODELBASE_H
