//
// Created by Keir Davis on 23/06/2026.
//

#ifndef MCPORT_GLU_H
#define MCPORT_GLU_H

#include "java/Type.h"

class GLU {
public:
    static void gluPerspective(float fovy, float aspect, float zNear, float zFar);

    static void gluPickMatrix(float x, float y, float width, float height, const int_t *viewport);
};


#endif //MCPORT_GLU_H
