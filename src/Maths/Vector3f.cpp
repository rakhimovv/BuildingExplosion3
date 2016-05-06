#include <math.h>
#include <cmath>
#include "Vector3f.h"

Vector3f::Vector3f() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Vector3f::Vector3f(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3f Vector3f::operator*=(float d) {
    this->x *= d;
    this->y *= d;
    this->z *= d;
    return *this;
}

Vector3f Vector3f::operator+=(Vector3f right) {
    this->x += right.x;
    this->y += right.y;
    this->z += right.z;
    return *this;
}

Vector3f Vector3f::operator-=(Vector3f right) {
    this->x -= right.x;
    this->y -= right.y;
    this->z -= right.z;
    return *this;
}

float Vector3f::SquareLength() {
    return x * x + y * y + z * z;
}

float Vector3f::Length() {
    return std::sqrt(SquareLength());
}

void Vector3f::Print() {
    std::cout << "x: " << this->x << "y: " << this->y << "z: " << this->z;
}

Vector3f Vector3f::xAxis() {
    return Vector3f(1.0f, 0.0f, 0.0f);
}

Vector3f Vector3f::yAxis() {
    return Vector3f(0.0f, 1.0f, 0.0f);
}

Vector3f Vector3f::zAxis() {
    return Vector3f(0.0f, 0.0f, 1.0f);
}


Vector3f Vector3f::random(float maxCoord) {
    return Vector3f(::random(-maxCoord, maxCoord), ::random(-maxCoord, maxCoord), ::random(-maxCoord, maxCoord));
}

float random(float minValue, float maxValue) {
    return minValue + (maxValue - minValue) * (float(rand()) / float(RAND_MAX));
}

Vector3f operator+(Vector3f left, Vector3f right) {
    Vector3f result;

    result.x = left.x + right.x;
    result.y = left.y + right.y;
    result.z = left.z + right.z;

    return result;
}

Vector3f operator-(Vector3f left, Vector3f right) {
    Vector3f result;

    result.x = left.x - right.x;
    result.y = left.y - right.y;
    result.z = left.z - right.z;

    return result;
}

Vector3f operator*(Vector3f v, float d) {
    Vector3f result;

    result.x = v.x * d;
    result.y = v.y * d;
    result.z = v.z * d;

    return result;
}

Vector3f operator*(float d, Vector3f v) {
    return v * d;
}

Vector3f operator^(Vector3f left, Vector3f right) {
    Vector3f result;

    result.x = left.y * right.z - left.z * right.y;
    result.y = left.z * right.x - left.x * right.z;
    result.z = left.x * right.y - left.y * right.x;

    return result;
}


float operator*(Vector3f v1, Vector3f v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z + v2.z;
}

Vector3f Vector3f::GetNorm() {
    return (*this) * (1.0f / Length());
}
