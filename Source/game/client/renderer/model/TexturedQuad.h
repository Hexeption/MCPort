//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_TEXTUREDQUAD_H
#define MCPORT_TEXTUREDQUAD_H

#include "PositionTexureVertex.h"

class TexturedQuad {
public:
    PositionTexureVertex vertexPositions[4];
    int nVertices = 4;

    TexturedQuad(PositionTexureVertex v0, PositionTexureVertex v1,
                 PositionTexureVertex v2, PositionTexureVertex v3);

    TexturedQuad(PositionTexureVertex v0, PositionTexureVertex v1,
                 PositionTexureVertex v2, PositionTexureVertex v3,
                 int u0, int v0i, int u1, int v1i);

    void flipFace();

    void draw(float scale) const;
};

#endif //MCPORT_TEXTUREDQUAD_H
