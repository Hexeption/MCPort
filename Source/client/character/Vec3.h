//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_VEC3_H
#define MCPORT_VEC3_H


class Vec3 {
public:
    float x;
    float y;
    float z;

    Vec3(float x, float y, float z);

    Vec3 interpolateTo(const Vec3 &t, float p);

    void set(float x, float y, float z);
};


#endif //MCPORT_VEC3_H
