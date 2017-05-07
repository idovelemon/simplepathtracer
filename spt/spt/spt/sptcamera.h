//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 04
// Brief: Use this camera to view the world
//----------------------------------------------------------
#ifndef _SPT_SPTCAMERA_H_
#define _SPT_SPTCAMERA_H_

#include <stdint.h>

#include "sptshape.h"
#include "sptvector.h"

namespace spt {

class Camera {
public:
    Camera(Vector3 pos, Vector3 target, float aspect, float fov,
        float dist, int32_t screen_width, int32_t screen_height);
    virtual ~Camera();

public:
    Vector3 GetPos();
    Vector3 GetTarget();
    float GetAspect();
    float GetFOV();
    float GetViewDist();

    Ray GenRay(int32_t x, int32_t y, Vector2 sampler);

private:
    Vector3     m_Pos;
    Vector3     m_Target;
    float       m_Aspect;
    float       m_FOV;
    float       m_Dist;
    int32_t     m_ScreenWidth;
    int32_t     m_ScreenHeight;

    float       m_ViewWidth;
    float       m_ViewHeight;
    Vector3     m_ViewCenter;
    Vector3     m_Right;
    Vector3     m_Up;
};

};  // namespace spt

#endif  // _SPT_SPTCAMERA_H_