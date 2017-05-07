//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 06
// Brief: Decription of shape
//----------------------------------------------------------
#ifndef _SPT_SPTSHAPE_H_
#define _SPT_SPTSHAPE_H_

#include <stdint.h>

#include "sptvector.h"

namespace spt {

class Ray {
public:
    Vector3 pos;
    Vector3 dir;
};

//--------------------------------------------------------

class Shape {
public:
    enum {
        SPHERE = 0,
        DISK,
        PLANE,
        MAX,
    };

public:
    Shape(int32_t type, float ep);
    virtual ~Shape();

public:
    virtual bool Hit(const Ray& r, Vector3& p, Vector3& n, float& t) = 0;

    int32_t GetType();
    float GetEp();

protected:
    int32_t             m_Type;
    float               m_Ep;
};

//--------------------------------------------------------

class Sphere : public Shape {
public:
    Sphere(Vector3 p, float r, float ep);
    virtual ~Sphere();

public:
    virtual bool Hit(const Ray& r, Vector3& p, Vector3& n, float& t);

protected:
    Vector3             m_Center;
    float               m_Radius;
};

//--------------------------------------------------------

class Disk : public Shape {
public:
    Disk(Vector3 p, Vector3 n, float r, float ep);
    virtual ~Disk();

public:
    virtual bool Hit(const Ray& r, Vector3& p, Vector3& n, float& t);

    float GetArea();
    Vector3 CalcPointLightPos(Vector2 sampler);

protected:
    Vector3         m_Center;
    Vector3         m_Normal;
    Vector3         m_Right;
    Vector3         m_Up;
    float           m_Radius;
};

//--------------------------------------------------------

class Plane : public Shape {
public:
    Plane(Vector3 p, Vector3 n, float ep);
    virtual ~Plane();

public:
    virtual bool Hit(const Ray& r, Vector3& p, Vector3& n, float& t);

protected:
    Vector3         m_Center;
    Vector3         m_Normal;
};

};  // namespace spt

#endif  // _SPT_SPTSHAPE_H_