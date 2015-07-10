#ifndef FVECTOR_H
#define FVECTOR_H

#define PI 3.1415926
#define DEGREE PI / 180.0f

class FVector3 {
public:
    float length() const;
    FVector3 normalize() const;

    FVector3 cross(const FVector3 &other) const;  // cross product
    FVector3 operator+ (const FVector3 &other) const;
    FVector3 operator- (const FVector3 &other) const;
    float operator* (const FVector3 &other) const;
    FVector3 operator* (const float r) const;
    FVector3 operator/ (const float r) const;

    void addBy(const FVector3 &other);  // add another vector to it
    void set(float x = 0.0, float y = 0.0, float z = 0.0) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float x, y, z;
};

#endif