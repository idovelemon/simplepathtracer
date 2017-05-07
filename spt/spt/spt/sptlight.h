//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 07
// Brief: The area light to lighting the world
//----------------------------------------------------------
#ifndef _SPT_SPTLIGHT_H_
#define _SPT_SPTLIGHT_H_

#include "sptvector.h"

namespace spt {

class Object;
class Sampler;

class AreaLight {
public:
    AreaLight(Object* obj, Sampler* sampler);
    virtual ~AreaLight();

public:
    float GetPDF();
    Vector3 GetLightColor();
    Sampler* GetSampler();
    Vector3 CalcPointLightPos(Vector2 sampler);

protected:
    Sampler*    m_Sampler;
    Object*     m_Object;
};

};  // namespace spt

#endif  // _SPT_SPTLIGHT_H_