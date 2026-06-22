//
// Created by Keir Davis on 22/06/2026.
//

#include "BufferUtils.h"

namespace lwjgl {
    namespace BufferUtils {
        FloatBuffer createFloatBuffer(std::size_t capacity) {
            return FloatBuffer(capacity);
        }

        IntBuffer createIntBuffer(std::size_t capacity) {
            return IntBuffer(capacity);
        }
    }
}
