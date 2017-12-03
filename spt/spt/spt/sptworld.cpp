//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 04
// Brief: Description the world that will be path traced
//----------------------------------------------------------
#include "sptworld.h"

#include <stdint.h>

#include "sptobject.h"

namespace spt {

World::World()
: m_MaxDepth(0)
, m_EnvLightColor(0.0f, 0.0f, 0.0f) {
    m_Objects.clear();
}

World::~World() {
}

void World::SetMaxDepth(int32_t depth) {
    m_MaxDepth = depth;
}

int32_t World::GetMaxDepth() const {
    return m_MaxDepth;
}

void World::SetEnvLightColor(Vector3 color) {
    m_EnvLightColor = color;
}

Vector3 World::GetEnvLightColor() const {
    return m_EnvLightColor;
}

void World::AddObject(Object* obj) {
    if (obj == NULL) return;
    m_Objects.push_back(obj);
}

void World::AddLight(AreaLight* light) {
    if (light == NULL) return;
    m_AreaLights.push_back(light);
}

ShadeBlock World::PrimaryTrace(const Ray& r) {
    ShadeBlock block;

    float min = FLT_MAX;
    Vector3 p(0.0f, 0.0f, 0.0f);
    Vector3 n(0.0f, 0.0f, 0.0f);
    float t = 0.0f;

    int32_t size = m_Objects.size();
    for (int32_t i = 0; i < size; i++) {
        if (m_Objects[i]->Hit(r, p, n, t)) {
            if (t < min) {
                if (Vector3::Dot(n, r.dir * (-1.0f)) > 0.0f) {
                    min = t;
                    block.SetPos(p);
                    block.SetNormal(n);
                    block.SetObject(m_Objects[i]);
                    block.SetLightDir(r.dir * (-1.0f));
                }
            }
        }
    }

    block.SetDepth(1);
    block.SetWorld(this);

    return block;
}

ShadeBlock World::SecondaryTrace(const Ray& r, const ShadeBlock& pre) {
    ShadeBlock block;

    float min = FLT_MAX;
    Vector3 p(0.0f, 0.0f, 0.0f);
    Vector3 n(0.0f, 0.0f, 0.0f);
    float t = 0.0f;

    int32_t size = m_Objects.size();
    for (int32_t i = 0; i < size; i++) {
        if (m_Objects[i]->Hit(r, p, n, t)) {
            if (t < min) {
                // Remove this check, assume all material are double face material
                //if (Vector3::Dot(n, r.dir * (-1.0f)) > 0.0f) {
                min = t;
                block.SetPos(p);
                block.SetNormal(n);
                block.SetObject(m_Objects[i]);
                block.SetLightDir(r.dir * (-1.0f));
            }
        }
    }

    block.SetDepth(pre.GetDepth() + 1);
    block.SetWorld(this);

    return block;
}

bool World::IsTwoPointVisible(Vector3 p0, Vector3 p1) {
    bool result = true;

    Ray r;
    r.pos = p0;
    r.dir = p1 - p0;
    r.dir.Normalize();

    float l = (p1 - p0).Length();

    Vector3 p(0.0f, 0.0f, 0.0f);
    Vector3 n(0.0f, 0.0f, 0.0f);
    float t = 0.0f;
    Object* obj = 0;

    int32_t size = m_Objects.size();
    for (int32_t i = 0; i < size; i++) {
        if (m_Objects[i]->Hit(r, p, n, t)) {
            if (t < l) {
                result = false;
                break;
            }
        }
    }

    return result;
}

std::vector<AreaLight*>& World::GetAreaLights() {
    return m_AreaLights;
}

};  // namespace spt