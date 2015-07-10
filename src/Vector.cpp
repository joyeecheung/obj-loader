#include <cmath>
#include "Vector.h"

float FVector3::length() const {
    return (float)(sqrt(x * x + y * y + z * z));
}

FVector3 FVector3::normalize() const {
    float len = length();
    FVector3 result;
    if (fabs(len - 0.0f) < 1e-6)
        result.set(0.0f, 0.0f, 0.0f);
    else
        result.set(x / len, y / len, z / len);
    return result;
}

FVector3 FVector3::cross(const FVector3 &v) const {
    FVector3 result;
    result.set(y * v.z - z * v.y,
               z * v.x - x * v.z,
               x * v.y - y * v.x);
    return result;
}

FVector3 FVector3::operator+ (const FVector3 &v) const {
    FVector3 result;
    result.set(x + v.x, y + v.y, z + v.z);
    return result;
}

FVector3 FVector3::operator- (const FVector3 &v) const {
    FVector3 result;
    result.set(x - v.x, y - v.y, z - v.z);
    return result;
}

float FVector3::operator* (const FVector3 &v) const {
    return x * v.x + y * v.y + z * v.z;
}

FVector3 FVector3::operator* (float r) const {
    FVector3 result;
    result.set(x * r, y * r, z * r);
    return result;
}

FVector3 FVector3::operator/ (float r) const {
    FVector3 result;
    result.set(x / r, y / r, z / r);
    return result;
}

void FVector3::addBy(const FVector3 &other) {
    x += other.x;
    y += other.y;
    z += other.z;
}