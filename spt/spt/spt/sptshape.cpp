//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 06
// Brief: Decription of shape
//----------------------------------------------------------
#include "sptshape.h"

#include <math.h>

namespace spt {

//----------------------------------------------------------

Shape::Shape(int32_t type, float ep)
: m_Type(type)
, m_Ep(ep) {
}

Shape::~Shape() {
}

int32_t Shape::GetType() {
    return m_Type;
}

float Shape::GetEp() {
    return m_Ep;
}

//----------------------------------------------------------

Sphere::Sphere(Vector3 p, float radius, float ep)
: Shape(Shape::SPHERE, ep)
, m_Center(p)
, m_Radius(radius) {
}

Sphere::~Sphere() {
}

bool Sphere::Hit(const Ray& r, Vector3& p, Vector3& n, float& t) {
    float a = Vector3::Dot(r.dir, r.dir);
    float b = 2 * Vector3::Dot(r.pos - m_Center, r.dir);
    float c = Vector3::Dot((r.pos - m_Center), (r.pos - m_Center)) - (m_Radius * m_Radius);
        
    float d = b * b - 4 * a * c;
    if (d < 0) {
        p = Vector3(0.0f, 0.0f, 0.0f);
        n = Vector3(0.0f, 0.0f, 0.0f);
        t = 0.0f;
        return false;
    } else if (d == 0) {
        t = -b / (2 * a);
        Vector3 q = r.pos + r.dir * t;
        if (t > m_Ep) {
            p = q;
            n = p - m_Center;
            n.Normalize();
            return true;
        } else {
            p = Vector3(0.0f, 0.0f, 0.0f);
            t = 0.0f;
            n = Vector3(0.0f, 0.0f, 0.0f);
            return false;
        }
    } else {
        float t0 = (-b + sqrt(d)) / (2 * a);
        float t1 = (-b - sqrt(d)) / (2 * a);
        if (t0 < m_Ep && t1 < m_Ep) {
            p = Vector3(0.0f, 0.0f, 0.0f);
            t = 0.0f;
            n = Vector3(0.0f, 0.0f, 0.0f);
            return false;
        } else if (t0 < m_Ep && t1 > m_Ep) {
            Vector3 q = r.pos + r.dir * t1;
            t = t1;
            p = q;
            n = p - m_Center;
            n.Normalize();
            return true;
        } else if (t0 > m_Ep && t1 < m_Ep) {
            Vector3 q = r.pos + r.dir * t0;
            t = t0;
            p = q;
            n = p - m_Center;
            n.Normalize();
            return true;
        } else {
            t = 0.0;
            if (t0 > t1) {
                t = t1;
            } else {
                t = t0;
            }
            Vector3 q = r.pos + r.dir * t;
            p = q;
            n = p - m_Center;
            n.Normalize();
            return true;
        }
    }
}

//----------------------------------------------------------

Disk::Disk(Vector3 p, Vector3 n, float size, float ep)
: Shape(Shape::DISK, ep)
, m_Center(p)
, m_Normal(n)
, m_Right(0.0f, 0.0f, 0.0f)
, m_Up(0.0f, 0.0f, 0.0f)
, m_Radius(size) {
    Vector3 up(0.0f, 1.0f, 0.0f);

    if (abs(Vector3::Dot(up, n)) < 1.0f) {
        m_Right = Vector3::Cross(up, n);
        m_Right.Normalize();
        m_Up = Vector3::Cross(n, m_Right);
        m_Up.Normalize();
    } else {
        if (n.y > 0.0f) {
            m_Right = Vector3(1.0f, 0.0f, 0.0f);
            m_Up = Vector3(0.0f, 0.0f, -1.0f);
        } else {
            m_Right = Vector3(-1.0f, 0.0f, 0.0f);
            m_Up = Vector3(0.0f, 0.0f, 1.0f);
        }
    }
}

Disk::~Disk() {
}

bool Disk::Hit(const Ray& r, Vector3& p, Vector3& n, float& t) {
    Vector3 target = m_Center - r.pos;
    float a = Vector3::Dot(target, m_Normal);
    float b = Vector3::Dot(r.dir, m_Normal);

    if (abs(b) < 0.0001) {
        p = Vector3(0.0f, 0.0f, 0.0f);
        n = Vector3(0.0f, 0.0f, 0.0f);
        t = 0.0f;
        return false;
    } else {
        t = a / b;
        if (t > m_Ep) {
            p = r.pos + r.dir * t;
            float to_center = (p - m_Center).Length();
            if (to_center < m_Radius) {
                n = m_Normal;
                return true;
            } else {
                p = Vector3(0.0f, 0.0f, 0.0f);
                n = Vector3(0.0f, 0.0f, 0.0f);
                t = 0.0f;
                return false;
            }
        } else {
            p = Vector3(0.0f, 0.0f, 0.0f);
            n = Vector3(0.0f, 0.0f, 0.0f);
            t = 0.0f;
            return false;
        }
    }
}

float Disk::GetArea() {
    return 3.141592f * m_Radius * m_Radius;
}

Vector3 Disk::GetNormal() {
    return m_Normal;
}

Vector3 Disk::CalcPointLightPos(Vector2 sampler) {
    sampler.x -= 0.5f;
    sampler.y -= 0.5f;

    Vector3 p = m_Center;
    p = p + m_Right * (sampler.x * 2.0f * m_Radius);
    p = p + m_Up * (sampler.y * 2.0f * m_Radius);

    return p;
}

//----------------------------------------------------------

Plane::Plane(Vector3 p, Vector3 n, float ep)
: Shape(Shape::PLANE, ep)
, m_Center(p)
, m_Normal(n) {
}

Plane::~Plane() {
}

bool Plane::Hit(const Ray& r, Vector3& p, Vector3& n, float& t) {
    Vector3 target = m_Center - r.pos;
    float a = Vector3::Dot(target, m_Normal);
    float b = Vector3::Dot(r.dir, m_Normal);
    if (abs(b) < 0.0001) {
        p = Vector3(0.0f, 0.0f, 0.0f);
        n = Vector3(0.0f, 0.0f, 0.0f);
        t = 0.0f;
        return false;
    } else {
        t = a / b;
        if (t > m_Ep) {
            p = r.pos + r.dir * t;
            n = m_Normal;
            return true;
        } else {
            p = Vector3(0.0f, 0.0f, 0.0f);
            n = Vector3(0.0f, 0.0f, 0.0f);
            t = 0.0f;
            return false;
        }
    }
}

};  // namespace spt