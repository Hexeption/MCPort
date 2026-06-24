//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_MOUSEHELPER_H
#define MCPORT_MOUSEHELPER_H
#include "java/Type.h"

class Minecraft;

class MouseHelper {
private:
    Minecraft &minecraft;

public:
    int_t deltaX;
    int_t deltaY;

    MouseHelper(Minecraft &minecraft);

    void grabMouseCursor();

    void ungrabMouseCursor();

    void mouseXYChange();
};


#endif //MCPORT_MOUSEHELPER_H
