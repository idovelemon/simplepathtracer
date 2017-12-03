//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 04
// Brief: Path tracer
//----------------------------------------------------------
#ifndef _SPT_SPTTRACER_H_
#define _SPT_SPTTRACER_H_

#include <stdint.h>

namespace spt {

class Camera;
class World;
class Sampler;

class Tracer {
public:
    Tracer(World* world, Camera* camera, Sampler* sampler);
    virtual ~Tracer();

public:
    void Trace(int32_t start_line, int32_t end_line,
        int32_t width, int32_t height,
        float* color_buf);
    void TraceOnePixel(int32_t x, int32_t y, int32_t width, int32_t height, float* color_buf);

private:
    Camera*     m_Camera;
    World*      m_World;
    Sampler*    m_Sampler;
};

};  // namespace spt

#endif  // _SPT_SPTTRACER_H_