//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_TESSELLATOR_H
#define MCPORT_TESSELLATOR_H

#include <vector>

#include "java/Type.h"

class Tessellator {
public:
    static Tessellator instance;

    void draw();

    void startDrawingQuads();

    void startDrawing(int_t mode);

    void setTextureUV(double u, double v);

    void setColorOpaque(int_t red, int_t green, int_t blue);

    void setColorOpaque_I(int_t color);

    void setColorOpaque_F(float red, float green, float blue);

    void setColorRGBA_F(float red, float green, float blue, float alpha);

    void disableColor();

    void addVertexWithUV(double x, double y, double z, double u, double v);

    void addVertex(double x, double y, double z);

private:
    struct Vertex {
        double x;
        double y;
        double z;
        double u;
        double v;
        bool hasTexture;
        int_t red;
        int_t green;
        int_t blue;
        int_t alpha;
        bool hasColor;
    };

    std::vector<Vertex> vertices;
    double textureU = 0.0;
    double textureV = 0.0;
    int_t red = 255;
    int_t green = 255;
    int_t blue = 255;
    int_t alpha = 255;
    bool hasTexture = false;
    bool hasColor = false;
    bool colorDisabled = false;
    bool isDrawing = false;
    int_t drawMode = 0;
};

#endif //MCPORT_TESSELLATOR_H
