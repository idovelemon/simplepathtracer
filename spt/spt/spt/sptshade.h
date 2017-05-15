//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 07
// Brief: Shade a pixel
//----------------------------------------------------------
#ifndef _SPT_SPTSHADE_H_
#define _SPT_SPTSHADE_H_

#include <stdint.h>

#include "sptvector.h"

namespace spt {

class Object;
class World;

class ShadeBlock {
public:
    ShadeBlock();
    virtual ~ShadeBlock();

public:
    Vector3 Shade(int32_t sampler_index);

    void SetDepth(int32_t depth);
    int32_t GetDepth() const;
    void SetPos(Vector3 pos);
    void SetNormal(Vector3 normal);
    void SetObject(Object* obj);
    void SetWorld(World* world);

protected:
    Vector3 Direct();
    Vector3 InDirect(int32_t sampler_index);

private:
    int32_t         m_Depth;
    Vector3         m_Pos;
    Vector3         m_Normal;
    Object*         m_Object;
    World*          m_World;
};

};  // namespace spt

#endif  // _SPT_SPTSHADE_H_