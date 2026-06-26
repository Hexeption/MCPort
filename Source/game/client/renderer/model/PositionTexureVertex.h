//
// Created by Keir Davis on 26/06/2026.
//

#ifndef MCPORT_POSITIONTEXUREVERTEX_H
#define MCPORT_POSITIONTEXUREVERTEX_H

class PositionTexureVertex {
public:
    float x, y, z;
    float u, v;

    PositionTexureVertex() : x(0), y(0), z(0), u(0), v(0) {}

    PositionTexureVertex(float x, float y, float z, float u, float v)
        : x(x), y(y), z(z), u(u), v(v) {}

    PositionTexureVertex setTexturePosition(float newU, float newV) const {
        return {x, y, z, newU, newV};
    }
};

#endif //MCPORT_POSITIONTEXUREVERTEX_H
