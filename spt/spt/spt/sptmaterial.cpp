//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 07
// Brief: Material to descript property of surface
//----------------------------------------------------------
#include "sptmaterial.h"

#include <random>

namespace spt {

//----------------------------------------------------------

Material::Material(int32_t type, int32_t sampler_num)
: m_Type(type)
, m_Sampler(sampler_num) {
    //m_Sampler.GenSamplers();
    //m_Sampler.MapToHemiSphere();
}

Material::~Material() {
}

int32_t Material::GetType() {
    return m_Type;
}

//----------------------------------------------------------

Diffuse::Diffuse(float kd, Vector3 cd, int32_t sampler_num)
: Material(Material::DIFFUSE, sampler_num)
, m_Kd(kd)
, m_Cd(cd) {
}

Diffuse::~Diffuse() {
}

Vector3 Diffuse::GetBRDF() {
    return m_Cd * (m_Kd / 3.141592f);
}

float Diffuse::GetPDF(Vector3 normal, Vector3 wi) {
    return Vector3::Dot(normal, wi) / 3.141592f;
}

Ray Diffuse::GetReflectRay(Vector3 pos, Vector3 normal, Vector3 wi, int32_t sampler_index) {
    // Get sampler point on hemisphere
    Vector2 sample = m_Sampler.GenSamplerInHemiSphere(sampler_index);

    // Calculate orthogonal basie
    Vector3 w = normal;
    Vector3 v(0.0072f, 1.0f, 0.0034f);  // Slightly jittered it
    Vector3 u = Vector3::Cross(v, w);
    u.Normalize();
    v = Vector3::Cross(w, u);
    v.Normalize();

    // Calculate the sampler point's position
    float cos_phi = cos(sample.x);
    float sin_phi = sin(sample.x);
    float cos_theta = cos(sample.y);
    float sin_theta = sin(sample.y);
    float pu = sin_theta * cos_phi;
    float pv = sin_theta * sin_phi;
    float pw = cos_theta;

    // Build the reflect ray
    Ray ray;
    ray.pos = pos;
    ray.dir = u * pu + v * pv + w * pw;
    ray.dir.Normalize();

    return ray;
}

Vector3 Diffuse::GetColor() {
    return m_Cd * m_Kd;
}

//----------------------------------------------------------

Emission::Emission(float ke, Vector3 ce)
: Material(Material::EMISSION, 0)
, m_Ke(ke)
, m_Ce(ce) {
}

Emission::~Emission() {
}

Vector3 Emission::GetBRDF() {
    return Vector3(1.0f, 1.0f, 1.0f);
}

float Emission::GetPDF(Vector3 normal, Vector3 wi) {
    return 0.0f;
}

Ray Emission::GetReflectRay(Vector3 pos, Vector3 normal, Vector3 wi, int32_t sampler_index) {
    return Ray();
}

Vector3 Emission::GetColor() {
    return m_Ce * m_Ke;
}

float Emission::GetKe() {
    return m_Ke;
}

Vector3 Emission::GetCe() {
    return m_Ce;
}

//----------------------------------------------------------

PerfectSpecular::PerfectSpecular(float ks, Vector3 cs)
: Material(Material::PERFECTSPECULAR, 0)
, m_Ks(ks)
, m_Cs(cs) {
}

PerfectSpecular::~PerfectSpecular() {
}

Vector3 PerfectSpecular::GetBRDF() {
    return m_Cs * m_Ks;
}

float PerfectSpecular::GetPDF(Vector3 normal, Vector3 wi) {
    return Vector3::Dot(normal, wi);
}

Ray PerfectSpecular::GetReflectRay(Vector3 pos, Vector3 normal, Vector3 wi, int32_t sampler_index) {
    Vector3 dir = normal * (Vector3::Dot(wi, normal) * 2.0f) - wi;
    dir.Normalize();
    Ray ray;
    ray.pos = pos;
    ray.dir = dir;

    return ray;
}

Vector3 PerfectSpecular::GetColor() {
    return m_Cs * m_Ks;
}

float PerfectSpecular::GetKs() {
    return m_Ks;
}

Vector3 PerfectSpecular::GetCs() {
    return m_Cs;
}

//----------------------------------------------------------

Transparent::Transparent(float ks, Vector3 cs, float ior, float russianRouletteK)
: Material(Material::TRANSPARENT, 0)
, m_Ks(ks)
, m_Cs(cs)
, m_IOR(ior)
, m_RussianRouletteK(russianRouletteK)

, m_IsTotalInternalReflection(false)
, m_FrenelFactor(0.0f)
, m_IsInsideTransparent(false) {
}

Transparent::~Transparent() {
}

Vector3 Transparent::GetBRDF() {
    return m_Cs * m_Ks;
}

Vector3 Transparent::GetBTDF() {
    return m_Cs * m_Ks;
}

float Transparent::GetPDF(Vector3 normal, Vector3 wi) {
    return Vector3::Dot(normal, wi);
}

Ray Transparent::GetReflectRay(Vector3 pos, Vector3 normal, Vector3 wi, int32_t sampler_index) {
    Vector3 dir = normal * (Vector3::Dot(wi, normal) * 2.0f) - wi;
    dir.Normalize();
    Ray ray;
    ray.pos = pos;
    ray.dir = dir;

    return ray;
}

Ray Transparent::GetTransimitRay(Vector3 pos, Vector3 normal, Vector3 wi, int32_t sampler_index) {
    const float indexOfRefractOfAir = 1.0f;
    float ior = m_IOR / indexOfRefractOfAir;
    if (IsInsideTransparent(normal, wi)) {
        normal = normal * (-1.0f);
        ior = indexOfRefractOfAir / m_IOR;
    }

    wi = wi * (-1.0f);
    float ci = Vector3::Dot(wi, normal);
    float ct = sqrt(1.0f - (1.0 - ci * ci) / (ior * ior));
    Ray ray;
    ray.pos = pos;

    ray.dir = wi * (1.0f / ior) - normal * (ct + 1.0f * ci / ior);
    ray.dir.Normalize();

    return ray;
}

Vector3 Transparent::GetColor() {
    return m_Cs * m_Ks;
}

float Transparent::GetRussianRouletteK() {
    return m_RussianRouletteK;
}

bool Transparent::IsTotalInternalReflection(Vector3 normal, Vector3 wi) {
    const float indexOfRefractOfAir = 1.0f;
    float ior = m_IOR / indexOfRefractOfAir;
    if (IsInsideTransparent(normal, wi)) {
        normal = normal * (-1.0f);
        ior = indexOfRefractOfAir / m_IOR;
    }

    bool result = false;
    float ior_inv = 1.0f / ior;
    wi = wi * (-1.0f);
    float c = Vector3::Dot(normal, wi);
    if (1.0f - ior_inv * ior_inv * (1.0f - c * c) < 0.0f) {
        result = true;
    }

    return result;
}

float Transparent::GetFrenelFactor(Vector3 normal, Vector3 wi) {
    if (IsInsideTransparent(normal, wi)) {
        normal = normal * (-1.0f);
    }

    const float indexOfRefractOfAir = 1.0f;
    float a = m_IOR - indexOfRefractOfAir;
    float b = m_IOR + indexOfRefractOfAir;
    float R0 = a * a / (b * b);
    float c = Vector3::Dot(normal, wi);
    c = 1.0f - c;
    return R0 + (1.0f - R0) * c * c * c * c * c;
}

bool Transparent::IsInsideTransparent(Vector3 normal, Vector3 wi) {
    bool result = false;

    if (Vector3::Dot(normal, wi) < 0.0f) {
        result = true;
    }

    return result;
}
};  // namespace spt