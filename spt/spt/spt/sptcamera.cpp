//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 04
// Brief: Use this camera to view the world
//----------------------------------------------------------
#include "sptcamera.h"

#include <math.h>

namespace spt {

Camera::Camera(Vector3 pos, Vector3 target, float aspect, float fov,
               float dist, int32_t screen_width, int32_t screen_height)
: m_Pos(pos)
, m_Target(target)
, m_Aspect(aspect)
, m_FOV(fov)
, m_Dist(dist)
, m_ScreenWidth(screen_width)
, m_ScreenHeight(screen_height)
, m_ViewWidth(0.0f)
, m_ViewHeight(0.0f)
, m_ViewCenter(0.0f, 0.0f, 0.0f)
, m_Right(0.0f, 0.0f, 0.0f)
, m_Up(0.0f, 0.0f, 0.0f) {
    float half_height = m_Dist * tan(m_FOV * 0.5f / 180.0f * 3.1415926f);
    float half_width = half_height * m_Aspect;
    m_ViewWidth = half_width * 2.0f;
    m_ViewHeight = half_height * 2.0f;

    Vector3 look = m_Target - m_Pos;
    look.Normalize();
    Vector3 ref_up(0.0f, 1.0f, 0.0f);
    m_Right = Vector3::Cross(ref_up, look);
    m_Right.Normalize();
    m_Up = Vector3::Cross(look, m_Right);
    m_Up.Normalize();

    m_ViewCenter = m_Pos + look * m_Dist;
}

Camera::~Camera() {
}

Vector3 Camera::GetPos() {
    return m_Pos;
}

Vector3 Camera::GetTarget() {
    return m_Target;
}

float Camera::GetAspect() {
    return m_Aspect;
}

float Camera::GetFOV() {
    return m_FOV;
}

float Camera::GetViewDist() {
    return m_Dist;
}

Ray Camera::GenRay(int32_t x, int32_t y, Vector2 sampler) {
    Ray r;
    r.pos = m_Pos;
    float x_pos = x * 1.0f - 0.5f + sampler.x;
    float y_pos = y * 1.0f - 0.5f + sampler.y;
    float x_ratio = x_pos / m_ScreenWidth;
    float y_ratio = y_pos / m_ScreenHeight;
    x_ratio = x_ratio - 0.5f;
    y_ratio = 0.5f - y_ratio;
    Vector3 target = m_ViewCenter + m_Right * (x_ratio * m_ViewWidth);
    target = target + m_Up * (y_ratio * m_ViewHeight);
    r.dir = target - m_Pos;
    r.dir.Normalize();

    return r;
}

};  // namespace spt