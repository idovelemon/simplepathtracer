//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 05
// Brief: Define some sampler method
//----------------------------------------------------------
#include "sptsampler.h"

#include <math.h>
#include <time.h>
#include <vector>

#include "sptutil.h"

namespace spt {

Sampler::Sampler(int32_t sampler_num)
: m_SamplerNum(sampler_num)
, m_Samplers(nullptr) {
    m_Samplers = new Vector2[sampler_num];
}

Sampler::~Sampler() {
    if (m_Samplers) {
        delete[] m_Samplers;
        m_Samplers = nullptr;
    }
}

void Sampler::GenSamplers() {
    float t = sqrt(1.0f * m_SamplerNum);
    if (abs(t - floor(t)) > 0.0001f) {
        printf("Error: Sampler number must be squre\n");
        return;
    }

    float step = 1.0f / m_SamplerNum;
        
    std::vector<std::vector<int32_t>> col_lists;
    std::vector<std::vector<int32_t>> row_lists;
    for(int32_t i = 0; i < static_cast<int32_t>(floor(t)); i++) {
        col_lists.push_back(std::vector<int32_t>());
        row_lists.push_back(std::vector<int32_t>());
        for(int32_t j = 0; j < static_cast<int32_t>(floor(t)); j++) {
            col_lists[i].push_back(j);
            row_lists[i].push_back(j);
        }
    }

    int32_t sampler_index = 0;
    for(int32_t i = 0; i < static_cast<int32_t>(floor(t)); i++) {
        for(int32_t j = 0; j < static_cast<int32_t>(floor(t)); j++) {
            int32_t col = col_lists[j][rand() % (col_lists[j].size())];
            int32_t row = row_lists[i][rand() % (row_lists[i].size())];
            float x = j * t + col;
            float y = i * t + row;
            x = x * step + (rand() % 100 / 100.0f) * step;
            y = y * step + (rand() % 100 / 100.0f) * step;
            m_Samplers[sampler_index] = Vector2(x, y);
            sampler_index++;
            col_lists[j].erase(std::find(col_lists[j].begin(), col_lists[j].end(), col));
            row_lists[i].erase(std::find(row_lists[i].begin(), row_lists[i].end(), row));
        }
    }
}

Vector2 Sampler::GenSamplerInHemiSphere() {
    Vector2 sampler;
    sampler.x = rand() % 100 / 100.0f;
    sampler.y = rand() % 100 / 100.0f;
    sampler.x = 2 * 3.141592f * sampler.x;
    float t = pow(1 - sampler.y, 0.5f);
    sampler.y = acos(t);
    return sampler;
}

void Sampler::MapToDisk() {
    // Use concentric map to map to disk
    for (int32_t i = 0; i < m_SamplerNum; i++) {
        // Map to [-1,1]
        Vector2 sp(2.0f * m_Samplers[i].x - 1.0f, 2.0f * m_Samplers[i].y - 1.0f);

        float r = 0.0f;
        float phi = 0.0f;
        if (sp.x > -sp.y) {  // sector 1 and 2
            if (sp.x > sp.y) {  // sector 1
                r = sp.x;
                phi = sp.y / sp.x;
            } else {  // sector 2
                r = sp.y;
                phi = 2.0f - sp.x / sp.y;
            }
        } else {  // sectors 3 and 4
            if (sp.x < sp.y) {  // sector 3
                r = -sp.x;
                phi = 4 + sp.y / sp.x;
            } else {  // sector 4
                r = -sp.y;
                if (sp.y != 0.0) {
                    phi = 6.0f - sp.x / sp.y;
                } else {
                    phi = 0.0f;
                }
            }
        }

        phi *= (3.141592f / 4.0f);
        m_Samplers[i].x = r * cos(phi);
        m_Samplers[i].y = r * sin(phi);
        m_Samplers[i].x = (m_Samplers[i].x + 1.0f) * 0.5f;
        m_Samplers[i].y = (m_Samplers[i].y + 1.0f) * 0.5f;
    }
}

void Sampler::MapToHemiSphere() {
    for (int32_t i = 0; i < m_SamplerNum; i++) {
        m_Samplers[i].x = 2 * 3.141592f * m_Samplers[i].x;
        float t = pow(1 - m_Samplers[i].y, 0.5f);
        m_Samplers[i].y = acos(t);
    }
}

int32_t Sampler::GetSamplerNum() {
    return m_SamplerNum;
}

Vector2* Sampler::GetSamplers() {
    return m_Samplers;
}

};  // namespace spt