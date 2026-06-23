//
// Created by Keir Davis on 22/06/2026.
//

#ifndef MCPORT_SYSTEM_H
#define MCPORT_SYSTEM_H
#include "String.h"
#include "Type.h"


namespace System {
    long_t currentTimeMillis();

    long_t nanoTime();

    jstring getProperty(const jstring &key);

    jstring getProperty(const jstring &key, const jstring &defaultValue);
}


#endif //MCPORT_SYSTEM_H
