//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 07
// Brief: Material to descript property of surface
//----------------------------------------------------------
#ifndef _SPT_SPTMATERIAL_H_
#define _SPT_SPTMATERIAL_H_

#include <stdint.h>

#include "sptvector.h"

namespace spt {

class Material {
public:
    enum {
        DIFFUSE,
        EMISSION,
    };

public:
    Material(int32_t type);
    virtual ~Material();

public:
    int32_t GetType();
    virtual Vector3 GetBRDF() = 0;

protected:
    int32_t     m_Type;
};

//----------------------------------------------------------

class Diffuse : public Material {
public:
    Diffuse(float kd, Vector3 cd);
    virtual ~Diffuse();

public:
    virtual Vector3 GetBRDF();

protected:
    float       m_Kd;
    Vector3     m_Cd;
};

//----------------------------------------------------------

class Emission : public Material {
public:
    Emission(float ke, Vector3 ce);
    virtual ~Emission();

public:
    virtual Vector3 GetBRDF();

    float GetKe();
    Vector3 GetCe();

protected:
    float       m_Ke;
    Vector3     m_Ce;
};

};  // namespace spt

#endif  // _SPT_SPTMATERIAL_H_