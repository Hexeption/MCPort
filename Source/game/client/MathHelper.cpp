//
// Created by Keir Davis on 24/06/2026.
//

#include "MathHelper.h"

#include <array>
#include <cmath>

static std::array<float, 65536> makeSinTable() {
    std::array<float, 65536> table{};
    for (int_t var0 = 0; var0 < static_cast<int_t>(table.size()); ++var0) {
        table[var0] = static_cast<float>(std::sin(static_cast<double>(var0) * std::acos(-1.0) * 2.0 / 65536.0));
    }
    return table;
}

static const std::array<float, 65536> SIN_TABLE = makeSinTable();

float MathHelper::sin(const float value) {
    return SIN_TABLE[static_cast<int_t>(value * 10430.378f) & 0xffff];
}

float MathHelper::cos(const float value) {
    return SIN_TABLE[static_cast<int_t>(value * 10430.378f + 16384.0f) & 0xffff];
}

float MathHelper::sqrt_float(const float value) {
    return std::sqrt(value);
}

float MathHelper::sqrt_double(const double value) {
    return static_cast<float>(std::sqrt(value));
}

int_t MathHelper::floor_float(const float value) {
    const int_t var1 = static_cast<int_t>(value);
    return value < static_cast<float>(var1) ? var1 - 1 : var1;
}

int_t MathHelper::floor_double(const double value) {
    const int_t var2 = static_cast<int_t>(value);
    return value < static_cast<double>(var2) ? var2 - 1 : var2;
}

float MathHelper::abs(const float value) {
    return value >= 0.0f ? value : -value;
}

double MathHelper::abs_max(double a, double b) {
    if (a < 0.0) {
        a = -a;
    }
    if (b < 0.0) {
        b = -b;
    }
    return a > b ? a : b;
}

int_t MathHelper::bucketInt(const int_t value, const int_t bucketSize) {
    return value < 0 ? -((-value - 1) / bucketSize) - 1 : value / bucketSize;
}
