//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_AABB_H
#define MCPORT_AABB_H

class AABB {
private:
    float epsilon = 0.0f;

public:
    float x0;
    float y0;
    float z0;
    float x1;
    float y1;
    float z1;

    AABB(float x0, float y0, float z0, float x1, float y1, float z1);

    AABB expand(float xa, float ya, float za);

    AABB grow(float xa, float ya, float za);

    float clipXCollide(AABB c, float xa);

    float clipYCollide(AABB c, float ya);

    float clipZCollide(AABB c, float za);

    bool intersects(AABB c);

    void move(float xa, float ya, float za);
};

#endif //MCPORT_AABB_H
