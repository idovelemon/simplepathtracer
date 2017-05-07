//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 07
// Brief: Shade a pixel
//----------------------------------------------------------
#include "sptshade.h"

#include <vector>

#include "sptlight.h"
#include "sptmaterial.h"
#include "sptobject.h"
#include "sptsampler.h"
#include "sptworld.h"

namespace spt {

ShadeBlock::ShadeBlock()
: m_Depth(0)
, m_Pos(0.0f, 0.0f, 0.0f)
, m_Normal(0.0f, 0.0f, 0.0f)
, m_Object(0)
, m_World(0) {
}

ShadeBlock::~ShadeBlock() {
    m_Object = 0;
    m_World = 0;
}

Vector3 ShadeBlock::Shade() {
    if (m_Object == 0) return Vector3(0.0f, 0.0f, 0.0f);

    Vector3 result_color(0.0f, 0.0f, 0.0f);

    if (m_Object->GetMaterial()->GetType() == Material::EMISSION) {
        // Emission
        Emission* emission = reinterpret_cast<Emission*>(m_Object->GetMaterial());
        result_color = emission->GetCe() * emission->GetKe();
        return result_color;
    } else {
        // None-Emission
        Vector3 result_direct = Direct();
        Vector3 result_indirect = InDirect();
        result_color = result_direct + result_indirect;
    }

    return result_color;
}

void ShadeBlock::SetDepth(int32_t depth) {
    m_Depth = depth;
}

void ShadeBlock::SetPos(Vector3 pos) {
    m_Pos = pos;
}

void ShadeBlock::SetNormal(Vector3 normal) {
    m_Normal = normal;
}

void ShadeBlock::SetObject(Object* obj) {
    m_Object = obj;
}

void ShadeBlock::SetWorld(World* world) {
    m_World = world;
}

Vector3 ShadeBlock::Direct() {
    Vector3 result_color(0.0f, 0.0f, 0.0f);

    std::vector<AreaLight*>& lights = m_World->GetAreaLights();
    int32_t size = lights.size();
    for (int32_t i = 0; i < size; i++) {
        AreaLight* light = lights[i];
        float pdf = light->GetPDF();
        Vector3 lc = light->GetLightColor();
        int32_t sampler_num = light->GetSampler()->GetSamplerNum();
        Vector3 result_area_light_color = Vector3(0.0f, 0.0f, 0.0f);

        // Resampling for good image quality
        light->GetSampler()->GenSamplers();
        light->GetSampler()->MapToDisk();
        Vector2* samplers = light->GetSampler()->GetSamplers();
        for (int32_t j = 0; j < sampler_num; j++) {
            Vector3 pos = light->CalcPointLightPos(samplers[j]);
            if (m_World->IsTwoPointVisible(m_Pos, pos)) {
                Vector3 brdf = m_Object->GetMaterial()->GetBRDF();

                Vector3 l = pos - m_Pos;
                float squre_length = l.SqureLength();
                l.Normalize();
                float cos = Vector3::Dot(l, m_Normal);
                cos = (0.0f > cos) ? 0.0f : cos;

                Vector3 out_light = brdf * lc * cos;  // Reflection Equation
                out_light = out_light * (1.0f / squre_length);  // Distance Attenuation
                Vector3 result_point_light_color = out_light * (1.0f / pdf);  // Monte Carlo Integration-Step1
                result_area_light_color = result_area_light_color + result_point_light_color;
            }
        }

        result_area_light_color = result_area_light_color * (1.0f / sampler_num);  // Monte Carlo Integration-Step2
        result_color = result_color + result_area_light_color;
    }

    return result_color;
}

Vector3 ShadeBlock::InDirect() {
    return Vector3(0.0f, 0.0f, 0.0f);
}

};  // namespace spt