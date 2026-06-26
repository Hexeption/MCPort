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

    void setColorRGBA(int_t red, int_t green, int_t blue, int_t alpha);

    void setColorOpaque_I(int_t color);

    void setColorOpaque_F(float red, float green, float blue);

    void setColorRGBA_F(float red, float green, float blue, float alpha);

    void disableColor();

    void setNormal(float x, float y, float z);

    void setTranslationD(double x, double y, double z);

    void setTranslationF(float x, float y, float z);

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
        float normalX;
        float normalY;
        float normalZ;
        bool hasNormal;
    };

    static constexpr bool convertQuadsToTriangles = true;

    std::vector<Vertex> vertices;

    double textureU = 0.0;
    double textureV = 0.0;

    int_t red = 255;
    int_t green = 255;
    int_t blue = 255;
    int_t alpha = 255;

    float normalX = 0.0f;
    float normalY = 0.0f;
    float normalZ = 0.0f;

    double xOffset = 0.0;
    double yOffset = 0.0;
    double zOffset = 0.0;

    bool hasTexture = false;
    bool hasColor = false;
    bool hasNormals = false;
    bool colorDisabled = false;
    bool isDrawing = false;

    int_t drawMode = 0;
    int_t addedVertices = 0;

    static int_t clampColor(int_t value);

    Vertex makeCurrentVertex(double x, double y, double z) const;
};

#endif //MCPORT_TESSELLATOR_H
