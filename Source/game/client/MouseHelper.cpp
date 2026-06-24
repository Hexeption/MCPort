//
// Created by Keir Davis on 24/06/2026.
//

#include "MouseHelper.h"

#include "lwjgl/Mouse.h"

// todo save mouse location and restore it make the gui better

MouseHelper::MouseHelper(Minecraft &minecraft) : minecraft(minecraft) {
}

void MouseHelper::grabMouseCursor() {
    lwjgl::Mouse::setGrabbed(true);
    deltaX = 0;
    deltaY = 0;
}

void MouseHelper::ungrabMouseCursor() {
    lwjgl::Mouse::setGrabbed(false);
    deltaX = 0;
    deltaY = 0;
}

void MouseHelper::mouseXYChange() {
    deltaX = lwjgl::Mouse::getDX();
    deltaY = lwjgl::Mouse::getDY();
}
