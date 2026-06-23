//
// Created by Keir Davis on 23/06/2026.
//

#include "Random.h"

#include <chrono>
#include <stdexcept>

static constexpr long_t MULTIPLIER = 0x5DEECE66DL;
static constexpr long_t ADDEND = 0xB;
static constexpr long_t MASK = (1L << 48) - 1;

Random::Random()
    : Random(std::chrono::duration_cast<std::chrono::nanoseconds>(
                  std::chrono::high_resolution_clock::now().time_since_epoch())
                  .count()) {}

Random::Random(long_t seed) {
    setSeed(seed);
}

void Random::setSeed(long_t seed) {
    this->seed = (seed ^ MULTIPLIER) & MASK;
}

int_t Random::next(int_t bits) {
    seed = (seed * MULTIPLIER + ADDEND) & MASK;
    return (int_t) (seed >> (48 - bits));
}

bool Random::nextBoolean() {
    return next(1) != 0;
}

int_t Random::nextInt() {
    return next(32);
}

int_t Random::nextInt(int_t bound) {
    if (bound <= 0) {
        throw std::invalid_argument("bound must be positive");
    }

    const int_t threshold = -bound % bound;
    int_t candidate = next(31);

    if ((bound & threshold) == 0) {
        return static_cast<int_t>((bound * static_cast<long_t>(candidate)) >> 31);
    }

    int_t result = candidate % bound;
    while (candidate - result + threshold < 0) {
        candidate = next(31);
        result = candidate % bound;
    }

    return result;
}

long_t Random::nextLong() {
    return (static_cast<long_t>(next(32)) << 32) + next(32);
}

float Random::nextFloat() {
    return next(24) / static_cast<float>(1LL << 24);
}

double Random::nextDouble() {
    return ((static_cast<long_t>(next(27)) << 27) + next(27)) / static_cast<double>(1LL << 54);
}
