//
// Created by Keir Davis on 24/06/2026.
//

#ifndef MCPORT_VEC3D_H
#define MCPORT_VEC3D_H

#include <memory>

#include "java/String.h"

class Vec3D {
public:
    double xCoord;
    double yCoord;
    double zCoord;

    static std::unique_ptr<Vec3D> createVectorHelper(double x, double y, double z);

    static void initialize();

    static std::unique_ptr<Vec3D> createVector(double x, double y, double z);

    Vec3D(double x, double y, double z);

    Vec3D setComponents(double x, double y, double z) const;

    std::unique_ptr<Vec3D> subtract(const Vec3D &other) const;

    std::unique_ptr<Vec3D> normalize() const;

    std::unique_ptr<Vec3D> crossProduct(const Vec3D &other) const;

    std::unique_ptr<Vec3D> addVector(double x, double y, double z) const;

    double distanceTo(const Vec3D &other) const;

    double squareDistanceTo(const Vec3D &other) const;

    double squareDistanceTo(double x, double y, double z) const;

    double lengthVector() const;

    std::unique_ptr<Vec3D> getIntermediateWithXValue(const Vec3D &other, double x) const;

    std::unique_ptr<Vec3D> getIntermediateWithYValue(const Vec3D &other, double y) const;

    std::unique_ptr<Vec3D> getIntermediateWithZValue(const Vec3D &other, double z) const;

    jstring toString() const;

    void rotateAroundX(float angle);

    void rotateAroundY(float angle);
};

#endif //MCPORT_VEC3D_H
