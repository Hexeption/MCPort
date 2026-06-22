//
// Created by Keir Davis on 22/06/2026.
//

#ifndef MCPORT_STRING_H
#define MCPORT_STRING_H

#include <string>

#include "Type.h"

typedef std::u16string jstring;

namespace String {
    jstring fromUtf8(const std::string &str);

    std::string toUtf8(const jstring &str);

    jstring toString(int_t v, int_t base = 10);

    jstring toString(long_t v, int_t base = 10);

    jstring toString(uint_t v, int_t base = 10);

    jstring toString(ulong_t v, int_t base = 10);

    jstring toString(float v);

    jstring toString(double v);
}


#endif //MCPORT_STRING_H
