//
// Created by Keir Davis on 22/06/2026.
//

#ifndef MCPORT_BUFFERUTILS_H
#define MCPORT_BUFFERUTILS_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <vector>

#include "java/Type.h"

namespace lwjgl {
    template<typename T>
    class Buffer {
    public:
        explicit Buffer(std::size_t capacity) : storage(capacity), positionValue(0), limitValue(capacity) {
        }

        void clear() {
            positionValue = 0;
            limitValue = storage.size();
        }

        void put(const T *values, std::size_t count) {
            if (positionValue + count > storage.size()) {
                throw std::out_of_range("buffer overflow");
            }

            for (std::size_t i = 0; i < count; ++i) {
                storage[positionValue + i] = values[i];
            }

            positionValue += count;
        }

        void put(std::initializer_list<T> values) {
            put(values.begin(), values.size());
        }

        Buffer &flip() {
            limitValue = positionValue;
            positionValue = 0;
            return *this;
        }

        void limit(std::size_t newLimit) {
            if (newLimit > storage.size()) {
                throw std::out_of_range("buffer limit out of range");
            }

            limitValue = newLimit;
            if (positionValue > limitValue) {
                positionValue = limitValue;
            }
        }

        void get(T *dest, std::size_t count) const {
            if (count > limitValue) {
                throw std::out_of_range("buffer read out of range");
            }

            for (std::size_t i = 0; i < count; ++i) {
                dest[i] = storage[i];
            }
        }

        void get(T *dest) const {
            get(dest, limitValue);
        }

        T get() {
            if (positionValue >= limitValue) {
                throw std::out_of_range("buffer underflow");
            }

            return storage[positionValue++];
        }

        std::size_t capacity() const {
            return storage.size();
        }

        std::size_t position() const {
            return positionValue;
        }

        std::size_t limit() const {
            return limitValue;
        }

        const T *data() const {
            return storage.data();
        }

        T *data() {
            return storage.data();
        }

        const T &operator[](std::size_t index) const {
            return storage[index];
        }

        T &operator[](std::size_t index) {
            return storage[index];
        }

    private:
        std::vector<T> storage;
        std::size_t positionValue;
        std::size_t limitValue;
    };

    using FloatBuffer = Buffer<float>;
    using IntBuffer = Buffer<int_t>;

    namespace BufferUtils {
        FloatBuffer createFloatBuffer(std::size_t capacity);

        IntBuffer createIntBuffer(std::size_t capacity);
    }
}

#endif //MCPORT_BUFFERUTILS_H
