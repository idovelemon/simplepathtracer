//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 04
// Brief: Descript object in world
//----------------------------------------------------------
#ifndef _SPT_SPTOBJECT_H_
#define _SPT_SPTOBJECT_H_

#include "sptshape.h"
#include "sptvector.h"

namespace spt {

class Material;
class Shape;

class Object {
public:
    Object();
    virtual ~Object();

    void SetShape(Shape* s);
    Shape* GetShape();

    void SetMaterial(Material* m);
    Material* GetMaterial();

    bool Hit(const Ray& r, Vector3& p, Vector3& n, float& t);

private:
    Shape*      m_Shape;
    Material*   m_Material;
};

};  // namespace spt

#endif  // _SPT_SPTOBJECT_H_