//
// Created by Keir Davis on 24/06/2026.
//

#include "Vec3D.h"

#include "game/client/MathHelper.h"

std::unique_ptr<Vec3D> Vec3D::createVectorHelper(const double x, const double y, const double z) {
    return std::make_unique<Vec3D>(x, y, z);
}

void Vec3D::initialize() {
}

std::unique_ptr<Vec3D> Vec3D::createVector(const double x, const double y, const double z) {
    return createVectorHelper(x, y, z);
}

Vec3D::Vec3D(double x, double y, double z) {
    if (x == -0.0) {
        x = 0.0;
    }
    if (y == -0.0) {
        y = 0.0;
    }
    if (z == -0.0) {
        z = 0.0;
    }

    xCoord = x;
    yCoord = y;
    zCoord = z;
}

Vec3D Vec3D::setComponents(double x, double y, double z) const {
    if (x == -0.0) {
        x = 0.0;
    }
    if (y == -0.0) {
        y = 0.0;
    }
    if (z == -0.0) {
        z = 0.0;
    }

    return Vec3D(x, y, z);
}

std::unique_ptr<Vec3D> Vec3D::subtract(const Vec3D &other) const {
    return createVector(other.xCoord - xCoord, other.yCoord - yCoord, other.zCoord - zCoord);
}

std::unique_ptr<Vec3D> Vec3D::normalize() const {
    const double length = MathHelper::sqrt_double(xCoord * xCoord + yCoord * yCoord + zCoord * zCoord);
    if (length < 1.0E-4) {
        return createVector(0.0, 0.0, 0.0);
    }

    return createVector(xCoord / length, yCoord / length, zCoord / length);
}

std::unique_ptr<Vec3D> Vec3D::crossProduct(const Vec3D &other) const {
    return createVector(yCoord * other.zCoord - zCoord * other.yCoord,
                        zCoord * other.xCoord - xCoord * other.zCoord,
                        xCoord * other.yCoord - yCoord * other.xCoord);
}

std::unique_ptr<Vec3D> Vec3D::addVector(const double x, const double y, const double z) const {
    return createVector(xCoord + x, yCoord + y, zCoord + z);
}

double Vec3D::distanceTo(const Vec3D &other) const {
    const double x = other.xCoord - xCoord;
    const double y = other.yCoord - yCoord;
    const double z = other.zCoord - zCoord;
    return MathHelper::sqrt_double(x * x + y * y + z * z);
}

double Vec3D::squareDistanceTo(const Vec3D &other) const {
    const double x = other.xCoord - xCoord;
    const double y = other.yCoord - yCoord;
    const double z = other.zCoord - zCoord;
    return x * x + y * y + z * z;
}

double Vec3D::squareDistanceTo(const double x, const double y, const double z) const {
    const double dx = x - xCoord;
    const double dy = y - yCoord;
    const double dz = z - zCoord;
    return dx * dx + dy * dy + dz * dz;
}

double Vec3D::lengthVector() const {
    return MathHelper::sqrt_double(xCoord * xCoord + yCoord * yCoord + zCoord * zCoord);
}

std::unique_ptr<Vec3D> Vec3D::getIntermediateWithXValue(const Vec3D &other, const double x) const {
    const double dx = other.xCoord - xCoord;
    const double dy = other.yCoord - yCoord;
    const double dz = other.zCoord - zCoord;
    if (dx * dx < 1.0E-7f) {
        return nullptr;
    }

    const double distance = (x - xCoord) / dx;
    if (distance < 0.0 || distance > 1.0) {
        return nullptr;
    }

    return createVector(xCoord + dx * distance, yCoord + dy * distance, zCoord + dz * distance);
}

std::unique_ptr<Vec3D> Vec3D::getIntermediateWithYValue(const Vec3D &other, const double y) const {
    const double dx = other.xCoord - xCoord;
    const double dy = other.yCoord - yCoord;
    const double dz = other.zCoord - zCoord;
    if (dy * dy < 1.0E-7f) {
        return nullptr;
    }

    const double distance = (y - yCoord) / dy;
    if (distance < 0.0 || distance > 1.0) {
        return nullptr;
    }

    return createVector(xCoord + dx * distance, yCoord + dy * distance, zCoord + dz * distance);
}

std::unique_ptr<Vec3D> Vec3D::getIntermediateWithZValue(const Vec3D &other, const double z) const {
    const double dx = other.xCoord - xCoord;
    const double dy = other.yCoord - yCoord;
    const double dz = other.zCoord - zCoord;
    if (dz * dz < 1.0E-7f) {
        return nullptr;
    }

    const double distance = (z - zCoord) / dz;
    if (distance < 0.0 || distance > 1.0) {
        return nullptr;
    }

    return createVector(xCoord + dx * distance, yCoord + dy * distance, zCoord + dz * distance);
}

jstring Vec3D::toString() const {
    return u"(" + String::toString(xCoord) + u", " + String::toString(yCoord) + u", " + String::toString(zCoord) +
           u")";
}

void Vec3D::rotateAroundX(const float angle) {
    const float cosine = MathHelper::cos(angle);
    const float sine = MathHelper::sin(angle);
    const double x = xCoord;
    const double y = yCoord * static_cast<double>(cosine) + zCoord * static_cast<double>(sine);
    const double z = zCoord * static_cast<double>(cosine) - yCoord * static_cast<double>(sine);
    xCoord = x;
    yCoord = y;
    zCoord = z;
}

void Vec3D::rotateAroundY(const float angle) {
    const float cosine = MathHelper::cos(angle);
    const float sine = MathHelper::sin(angle);
    const double x = xCoord * static_cast<double>(cosine) + zCoord * static_cast<double>(sine);
    const double y = yCoord;
    const double z = zCoord * static_cast<double>(cosine) - xCoord * static_cast<double>(sine);
    xCoord = x;
    yCoord = y;
    zCoord = z;
}
