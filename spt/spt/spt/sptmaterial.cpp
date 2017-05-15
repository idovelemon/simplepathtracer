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

Ray Diffuse::GetReflectRay(Vector3 pos, Vector3 normal, int32_t sampler_index) {
    // Get sampler point on hemisphere
    Vector2 sample = m_Sampler.GenSamplerInHemiSphere();

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

    Vector3 target = pos + u * pu + v * pv + w * pw;

    // Build the reflect ray
    Ray ray;
    ray.pos = pos;
    ray.dir = (target - pos);
    ray.dir.Normalize();
    return ray;
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

Ray Emission::GetReflectRay(Vector3 pos, Vector3 normal, int32_t sampler_index) {
    return Ray();
}

float Emission::GetKe() {
    return m_Ke;
}

Vector3 Emission::GetCe() {
    return m_Ce;
}

};  // namespace spt