//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 04
// Brief: Description the world that will be path traced
//----------------------------------------------------------
#ifndef _SPT_SPTWORLD_H_
#define _SPT_SPTWORLD_H_

#include <vector>

#include "sptshade.h"
#include "sptshape.h"

namespace spt {

class AreaLight;
class Object;

class World {
public:
    World();
    virtual~ World();

public:
    void SetMaxDepth(int32_t depth);
    int32_t GetMaxDepth() const;

    void SetEnvLightColor(Vector3 color);
    Vector3 GetEnvLightColor() const;

    void AddObject(Object* obj);
    void AddLight(AreaLight* light);

    ShadeBlock PrimaryTrace(const Ray& r);
    ShadeBlock SecondaryTrace(const Ray& r, const ShadeBlock& pre);

    bool IsTwoPointVisible(Vector3 p0, Vector3 p1);

    std::vector<AreaLight*>& GetAreaLights();

private:
    int32_t                     m_MaxDepth;
    Vector3                     m_EnvLightColor;
    std::vector<Object*>        m_Objects;
    std::vector<AreaLight*>     m_AreaLights;
};

};

#endif  // _SPT_SPTWORLD_H_