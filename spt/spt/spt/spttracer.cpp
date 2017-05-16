//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 04
// Brief: Path tracer
//----------------------------------------------------------
#include "spttracer.h"

#include <stdint.h>
#include <time.h>
#include <iostream>

#include"sptcamera.h"
#include "sptsampler.h"
#include "sptshade.h"
#include "sptshape.h"
#include "sptutil.h"
#include "sptworld.h"

namespace spt {

Tracer::Tracer(World* world, Camera* camera, Sampler* sampler)
: m_Camera(camera)
, m_World(world)
, m_Sampler(sampler) {
}

Tracer::~Tracer() {
    m_Camera = NULL;
    m_World = NULL;
    m_Sampler = NULL;
}

void Tracer::Trace(int32_t start_line, int32_t end_line,
                   int32_t width, int32_t height,
                   float* color_buf) {
    int32_t sampler_num = m_Sampler->GetSamplerNum();

    for (int32_t y = start_line; y < end_line; y++) {
        for (int32_t x = 0; x < width; x++) {
            Vector3 color(0.0f, 0.0f, 0.0f);

            // Resampling for good image quality
            m_Sampler->GenSamplers();
            Vector2* samplers = m_Sampler->GetSamplers();
            for (int32_t j = 0; j < sampler_num; j++) {
                Ray ray = m_Camera->GenRay(x, y, samplers[j]);
                ShadeBlock block = m_World->PrimaryTrace(ray);
                color = color + block.Shade(j);
            }
            color = color * (1.0f / sampler_num);
            color.x = Clamp(color.x);
            color.y = Clamp(color.y);
            color.z = Clamp(color.z);
            //ToneMapping(color.x, color.y, color.z);
            SetRGB(color_buf, x, y, width, height, color.x, color.y, color.z);
            printf("Pos:(%d,%d) Color:(%f,%f,%f)\n", x, y, color.x, color.y, color.z);
        }
    }
}

};  // namespace spt