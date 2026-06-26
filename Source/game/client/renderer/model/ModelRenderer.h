//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_MODELRENDERER_H
#define MCPORT_MODELRENDERER_H

#include <vector>

#include "TexturedQuad.h"

class ModelRenderer {
public:
    float rotationPointX = 0.0f;
    float rotationPointY = 0.0f;
    float rotationPointZ = 0.0f;
    float rotateAngleX = 0.0f;
    float rotateAngleY = 0.0f;
    float rotateAngleZ = 0.0f;
    bool mirror = false;
    bool showModel = true;
    bool isHidden = false;

    ModelRenderer(int texOffsetX, int texOffsetY);
    ~ModelRenderer();

    void addBox(float x, float y, float z, int w, int h, int d, float extra = 0.0f);

    void setRotationPoint(float px, float py, float pz);

    void render(float scale);

    void renderWithRotation(float scale);

private:
    int textureOffsetX;
    int textureOffsetY;
    std::vector<PositionTexureVertex> corners;
    std::vector<TexturedQuad> faces;
    bool compiled = false;
    unsigned int displayList = 0;

    void compileDisplayList(float scale);
};

#endif //MCPORT_MODELRENDERER_H
