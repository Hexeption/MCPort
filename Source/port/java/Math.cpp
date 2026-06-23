//
// Created by Keir Davis on 23/06/2026.
//

#include "Math.h"

namespace Math {
    double random() {
        auto random = Random();
        return random.nextDouble();
    }
}
