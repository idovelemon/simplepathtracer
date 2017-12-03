//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 07
// Brief: Shade a pixel
//----------------------------------------------------------
#include "sptshade.h"

#include <vector>
#include <time.h>

#include "sptlight.h"
#include "sptmaterial.h"
#include "sptobject.h"
#include "sptsampler.h"
#include "sptworld.h"

namespace spt {

extern double DRand48(void);

ShadeBlock::ShadeBlock()
: m_Depth(0)
, m_Pos(0.0f, 0.0f, 0.0f)
, m_Normal(0.0f, 0.0f, 0.0f)
, m_LightDir(0.0f, 0.0f, 0.0f)
, m_Object(0)
, m_World(0) {
}

ShadeBlock::~ShadeBlock() {
    m_Object = 0;
    m_World = 0;
}

Vector3 ShadeBlock::Shade(int32_t sampler_index) {
    // Out condition: No object has been traced
    if (m_Object == 0) return m_World->GetEnvLightColor();

    Vector3 result_color(0.0f, 0.0f, 0.0f);

    if (m_Object->GetMaterial()->GetType() == Material::EMISSION) {
        // Emission
        Emission* emission = reinterpret_cast<Emission*>(m_Object->GetMaterial());
        result_color = emission->GetCe() * emission->GetKe();
    } else {
        // None-Emission
        Vector3 result_indirect = InDirect(sampler_index);
        result_color = result_indirect;
    }

    return result_color;
}

void ShadeBlock::SetDepth(int32_t depth) {
    m_Depth = depth;
}

int32_t ShadeBlock::GetDepth() const {
    return m_Depth;
}

void ShadeBlock::SetPos(Vector3 pos) {
    m_Pos = pos;
}

void ShadeBlock::SetNormal(Vector3 normal) {
    m_Normal = normal;
}

Vector3 ShadeBlock::GetNormal() {
    return m_Normal;
}

void ShadeBlock::SetLightDir(Vector3 dir) {
    m_LightDir = dir;
}

Vector3 ShadeBlock::GetLightDir() {
    return m_LightDir;
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
            Vector3 ld = m_Pos - pos;
            ld.Normalize();
            if (Vector3::Dot(light->GetLightDir(), ld) > 0.0f) {
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
        }

        result_area_light_color = result_area_light_color * (1.0f / sampler_num);  // Monte Carlo Integration-Step2
        result_color = result_color + result_area_light_color;
    }

    return result_color;
}

Vector3 ShadeBlock::InDirect(int32_t sampler_index) {
    // Out Condition: Meet the max recursion
    //if (m_Depth > m_World->GetMaxDepth()) {
    //    return m_World->GetEnvLightColor();
    //}

    // Russian Roulette
    Vector3 color = m_Object->GetMaterial()->GetColor();
    float terminateRayP = 1.0f;
    if (m_Depth > m_World->GetMaxDepth()) {
        terminateRayP = color.x > color.y && color.x > color.z ? color.x : color.y > color.z ? color.y : color.z;
        if (DRand48() > terminateRayP) {
            // Terminate
            return m_Object->GetMaterial()->GetColor();
        }
    }

    bool castTransimittedRay = false;
    float transparentRayP = 1.0f;
    float frenelFactor = 1.0f;
    bool insideTransparent = false;
    bool totalInternalReflect = false;
    Ray transimitRay;
    if (m_Object->GetMaterial()->GetType() == Material::TRANSPARENT) {
        Transparent* material = static_cast<Transparent*>(m_Object->GetMaterial());
        totalInternalReflect = material->IsTotalInternalReflection(m_Normal, m_LightDir);
        transimitRay = m_Object->GetMaterial()->GetTransimitRay(m_Pos, m_Normal, m_LightDir, sampler_index);

        // Calculate frenel factor ?????????
        if (!totalInternalReflect) {
            if (insideTransparent) {
                frenelFactor = material->GetFrenelFactor(m_Normal, transimitRay.dir * (-1.0f));
            } else {
                frenelFactor = material->GetFrenelFactor(m_Normal, m_LightDir);
            }
        }

        // Use russian roulette to choose reflection | transmition ray
        if (!totalInternalReflect) {
            float k = material->GetRussianRouletteK();
            transparentRayP = k / 2.0f + (1.0f - k) * frenelFactor;
            if (DRand48() > transparentRayP) {
                castTransimittedRay = true;
            }
        }

        // Reverse normal
        insideTransparent = material->IsInsideTransparent(m_Normal, m_LightDir);
        if (insideTransparent) {
            m_Normal = m_Normal * (-1.0f);
        }
    }

    if (m_Depth <= 2 && !totalInternalReflect && m_Object->GetMaterial()->GetType() == Material::TRANSPARENT) {
        // Shoot secondary ray
        Ray reflectRay = m_Object->GetMaterial()->GetReflectRay(m_Pos, m_Normal, m_LightDir, sampler_index);
        ShadeBlock reflectShade = m_World->SecondaryTrace(reflectRay, *this);

        ShadeBlock transimitShade = m_World->SecondaryTrace(transimitRay, *this);

        // Calculate the indirect light
        Vector3 reflectLc = reflectShade.Shade(sampler_index);
        Vector3 transimitLc = transimitShade.Shade(sampler_index);

        // Calculate material color
        Vector3 brdf = m_Object->GetMaterial()->GetBRDF();
        float reflectPdf = m_Object->GetMaterial()->GetPDF(m_Normal, reflectRay.dir);
        float reflectCos = Vector3::Dot(m_Normal, reflectRay.dir);
        reflectCos = (0.0f > reflectCos) ? 0.0f : reflectCos;
        Vector3 reflectColor = brdf * reflectLc * (frenelFactor * reflectCos / reflectPdf / terminateRayP);  // Reflection Equation

        Vector3 btdf = m_Object->GetMaterial()->GetBTDF();
        float transimitPdf = m_Object->GetMaterial()->GetPDF(m_Normal * (-1.0f), transimitRay.dir);
        float transmitCos = Vector3::Dot(m_Normal * (-1.0f), transimitRay.dir);
        transmitCos = (0.0f > transmitCos) ? 0.0f : transmitCos;
        Vector3 transmitColor = btdf * transimitLc * ((1.0f - frenelFactor) * transmitCos / transimitPdf / terminateRayP);  // Transmition Equation

        return reflectColor + transmitColor;
    } else if (castTransimittedRay) {
        // Shoot secondary ray
        ShadeBlock shade = m_World->SecondaryTrace(transimitRay, *this);

        // Calculate the indirect light
        Vector3 lc = shade.Shade(sampler_index);

        Vector3 btdf = m_Object->GetMaterial()->GetBTDF();
        float pdf = m_Object->GetMaterial()->GetPDF(m_Normal * (-1.0f), transimitRay.dir);
        float cos = Vector3::Dot(m_Normal * (-1.0f), transimitRay.dir);
        cos = (0.0f > cos) ? 0.0f : cos;
        return btdf * lc * ((1.0f - frenelFactor) * cos / pdf / terminateRayP / (1.0f - transparentRayP));  // Transmition Equation
    } else {
        // Shoot secondary ray
        Ray ray = m_Object->GetMaterial()->GetReflectRay(m_Pos, m_Normal, m_LightDir, sampler_index);
        ShadeBlock shade = m_World->SecondaryTrace(ray, *this);

        // Calculate the indirect light
        Vector3 lc = shade.Shade(sampler_index);

        Vector3 brdf = m_Object->GetMaterial()->GetBRDF();
        float pdf = m_Object->GetMaterial()->GetPDF(m_Normal, ray.dir);
        float cos = Vector3::Dot(m_Normal, ray.dir);
        cos = (0.0f > cos) ? 0.0f : cos;
        return brdf * lc * (frenelFactor * cos / pdf / terminateRayP / transparentRayP);  // Reflection Equation
    }
}

};  // namespace spt