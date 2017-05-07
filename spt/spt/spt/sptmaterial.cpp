//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 07
// Brief: Material to descript property of surface
//----------------------------------------------------------
#include "sptmaterial.h"

namespace spt {

//----------------------------------------------------------

Material::Material(int32_t type)
: m_Type(type) {
}

Material::~Material() {
}

int32_t Material::GetType() {
    return m_Type;
}

//----------------------------------------------------------

Diffuse::Diffuse(float kd, Vector3 cd)
: Material(Material::DIFFUSE)
, m_Kd(kd)
, m_Cd(cd) {
}

Diffuse::~Diffuse() {
}

Vector3 Diffuse::GetBRDF() {
    return m_Cd * (m_Kd / 3.141592f);
}

//----------------------------------------------------------

Emission::Emission(float ke, Vector3 ce)
: Material(Material::EMISSION)
, m_Ke(ke)
, m_Ce(ce) {
}

Emission::~Emission() {
}

Vector3 Emission::GetBRDF() {
    return Vector3(1.0f, 1.0f, 1.0f);
}

float Emission::GetKe() {
    return m_Ke;
}

Vector3 Emission::GetCe() {
    return m_Ce;
}

};  // namespace spt