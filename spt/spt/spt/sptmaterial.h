//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 07
// Brief: Material to descript property of surface
//----------------------------------------------------------
#ifndef _SPT_SPTMATERIAL_H_
#define _SPT_SPTMATERIAL_H_

#include <stdint.h>

#include "sptsampler.h"
#include "sptshape.h"
#include "sptvector.h"

namespace spt {

class Material {
public:
    enum {
        DIFFUSE,
        EMISSION,
        PERFECTSPECULAR,
        TRANSPARENT,
    };

public:
    Material(int32_t type, int32_t sampler_num);
    virtual ~Material();

public:
    int32_t GetType();
    virtual Vector3 GetBRDF() = 0;
    virtual Vector3 GetBTDF() { return Vector3(0.0f, 0.0f, 0.0f); };
    virtual float GetPDF(Vector3 normal, Vector3 wi) = 0;
    virtual Ray GetReflectRay(Vector3 pos, Vector3 normal, Vector3 wi, int32_t sampler_index) = 0;
    virtual Ray GetTransimitRay(Vector3 pos, Vector3 normal, Vector3 wi, int32_t sampler_index) { return Ray(); };
    virtual Vector3 GetColor() = 0;

protected:
    int32_t     m_Type;
    Sampler     m_Sampler;
};

//----------------------------------------------------------

class Diffuse : public Material {
public:
    Diffuse(float kd, Vector3 cd, int32_t sampler_num);
    virtual ~Diffuse();

public:
    virtual Vector3 GetBRDF();
    virtual float GetPDF(Vector3 normal, Vector3 wi);
    virtual Ray GetReflectRay(Vector3 pos, Vector3 normal, Vector3 wi, int32_t sampler_index);
    virtual Vector3 GetColor();

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
    virtual float GetPDF(Vector3 normal, Vector3 wi);
    virtual Ray GetReflectRay(Vector3 pos, Vector3 normal, Vector3 wi, int32_t sampler_index);
    virtual Vector3 GetColor();

    float GetKe();
    Vector3 GetCe();

protected:
    float       m_Ke;
    Vector3     m_Ce;
};


//----------------------------------------------------------

class PerfectSpecular : public Material {
public:
    PerfectSpecular(float ks, Vector3 cs);
    virtual ~PerfectSpecular();

public:
    virtual Vector3 GetBRDF();
    virtual float GetPDF(Vector3 normal, Vector3 wi);
    virtual Ray GetReflectRay(Vector3 pos, Vector3 normal, Vector3 wi, int32_t sampler_index);
    virtual Vector3 GetColor();

    float GetKs();
    Vector3 GetCs();

protected:
    float       m_Ks;
    Vector3     m_Cs;
};

//--------------------------------------------------------------

class Transparent : public Material {
public:
    Transparent(float ks, Vector3 cs, float ior, float russianRouletteK);
    virtual ~Transparent();

public:
    virtual Vector3 GetBRDF();
    virtual Vector3 GetBTDF();
    virtual float GetPDF(Vector3 normal, Vector3 wi);
    virtual Ray GetReflectRay(Vector3 pos, Vector3 normal, Vector3 wi, int32_t sampler_index);
    virtual Ray GetTransimitRay(Vector3 pos, Vector3 normal, Vector3 wi, int32_t sampler_index);
    virtual Vector3 GetColor();

    float GetRussianRouletteK();

public:
    bool IsTotalInternalReflection(Vector3 normal, Vector3 wi);
    float GetFrenelFactor(Vector3 normal, Vector3 wi);
    bool IsInsideTransparent(Vector3 normal, Vector3 wi);

protected:
    float m_Ks;
    Vector3 m_Cs;
    float m_IOR;
    float m_RussianRouletteK;

    bool m_IsTotalInternalReflection;
    float m_FrenelFactor;
    bool m_IsInsideTransparent;
};

};  // namespace spt

#endif  // _SPT_SPTMATERIAL_H_