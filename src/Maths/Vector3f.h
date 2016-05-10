#pragma once

#include <iostream>

float random(float minValue, float maxValue);

struct Vector3f {
    float x, y, z;

    Vector3f();

    Vector3f(float x, float y, float z);

    Vector3f operator*=(float d);

    Vector3f operator+=(Vector3f right);

    Vector3f operator-=(Vector3f right);

    float SquareLength();

    float Length();

    Vector3f GetNorm();

    void Print();

    static Vector3f xAxis();

    static Vector3f yAxis();

    static Vector3f zAxis();

    static Vector3f random(float maxCoord);
};

Vector3f operator+(Vector3f left, Vector3f right);

Vector3f operator-(Vector3f left, Vector3f right);

Vector3f operator*(Vector3f v, float d);

Vector3f operator*(float d, Vector3f v);

Vector3f operator^(Vector3f v0, Vector3f v1);

float operator*(Vector3f v1, Vector3f v2);
