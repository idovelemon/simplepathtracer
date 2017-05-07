//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 04
// Brief: math tool
//----------------------------------------------------------
#ifndef _SPT_SPTVECTOR_H_
#define _SPT_SPTVECTOR_H_

#include <math.h>

namespace spt {

class Vector2 {
public:
    float x;
    float y;

public:
    Vector2()
    : x(0)
    , y(0) {
    }

    Vector2(float _x, float _y)
    : x(_x)
    , y(_y) {
    }

};

class Vector3 {
public:
    float x;
    float y;
    float z;

public:
    Vector3()
    : x(0.0f)
    , y(0.0f)
    , z(0.0f) {
    }

    Vector3(float _x, float _y, float _z)
    : x(_x)
    , y(_y)
    , z(_z) {
    }

    inline Vector3 operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    inline Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    inline Vector3 operator*(const float t) const {
        return Vector3(x * t, y * t, z * t);
    }

    inline Vector3 operator*(const Vector3& v) const {
        return Vector3(x * v.x, y * v.y, z * v.z);
    }

    inline float Length() {
        return sqrtf(x * x + y * y + z * z);
    }

    inline float SqureLength() {
        return (x * x + y * y + z * z);
    }

    inline void Normalize() {
        float len = Length();
        x /= len;
        y /= len;
        z /= len;
    }

    static Vector3 Cross(const Vector3& v0, const Vector3& v1) {
        return Vector3(v0.y * v1.z - v0.z * v1.y,
                      v0.z * v1.x - v0.x * v1.z,
                      v0.x * v1.y - v0.y * v1.x);
    }

    static float Dot(const Vector3& v0, const Vector3& v1) {
        return (v0.x * v1.x + v0.y * v1.y + v0.z * v1.z);
    }
};

};  // namespace spt

#endif  // _SPT_SPTVECTOR_H_