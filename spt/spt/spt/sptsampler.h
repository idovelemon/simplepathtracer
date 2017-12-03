//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 05
// Brief: Define some sampler method
//----------------------------------------------------------
#ifndef _SPT_SPTSAMPLER_H_
#define _SPT_SPTSAMPLER_H_

#include <stdint.h>

#include "sptvector.h"

namespace spt {

class Sampler {
public:
    Sampler(int32_t sampler_num);
    virtual ~Sampler();

public:
    void GenSamplers();
    Vector2 GenSamplerInHemiSphere(int32_t index);
    void MapToDisk();
    void MapToHemiSphere();

    int32_t GetSamplerNum();
    Vector2* GetSamplers();

private:
    int32_t     m_SamplerNum;
    Vector2*    m_Samplers;
};

};  // namespace spt

#endif  // _SPT_SPTSAMPLER_H_