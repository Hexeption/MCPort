//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_KEYBINDING_H
#define MCPORT_KEYBINDING_H
#include "java/String.h"


class Keybinding {
public:
    jstring keyDescription;
    int_t key;

    Keybinding(jstring keyDescription, int_t key) : keyDescription(keyDescription), key(key) {
    }
};


#endif //MCPORT_KEYBINDING_H
