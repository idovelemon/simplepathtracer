//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 07
// Brief: The area light to lighting the world
//----------------------------------------------------------
#include "sptlight.h"

#include "sptmaterial.h"
#include "sptobject.h"
#include "sptsampler.h"

namespace spt {

AreaLight::AreaLight(Object* obj, Sampler* sampler)
: m_Sampler(sampler)
, m_Object(obj) {
    sampler->GenSamplers();
    sampler->MapToDisk();
}

AreaLight::~AreaLight() {
    m_Sampler = 0;
    m_Object = 0;
}

float AreaLight::GetPDF() {
    float pdf = 0.0f;

    // Only support disk area light now
    if (m_Object->GetShape()->GetType() == Shape::DISK) {
        Disk* disk = reinterpret_cast<Disk*>(m_Object->GetShape());
        pdf = 1.0f / disk->GetArea();
    }

    return pdf;
}

Vector3 AreaLight::GetLightColor() {
    Vector3 result(0.0f, 0.0f, 0.0f);

    // Must be emission material
    if (m_Object->GetMaterial()->GetType() == Material::EMISSION) {
        Emission* emission = reinterpret_cast<Emission*>(m_Object->GetMaterial());
        result = emission->GetCe() * emission->GetKe();
    }

    return result;
}

Sampler* AreaLight::GetSampler() {
    return m_Sampler;
}

Vector3 AreaLight::CalcPointLightPos(Vector2 sampler) {
    Vector3 result(0.0f, 0.0f, 0.0f);

    // Only support disk area light now
    if (m_Object->GetShape()->GetType() == Shape::DISK) {
        Disk* disk = reinterpret_cast<Disk*>(m_Object->GetShape());
        result = disk->CalcPointLightPos(sampler);
    }

    return result;
}

};  // namespace spt