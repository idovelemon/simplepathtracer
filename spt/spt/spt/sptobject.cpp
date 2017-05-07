//----------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 04
// Brief: Descript object in world
//----------------------------------------------------------
#include "sptobject.h"

#include "sptshape.h"

namespace spt {

Object::Object()
: m_Shape(0)
, m_Material(0) {
}

Object::~Object() {
    m_Shape = 0;
    m_Material = 0;
}

void Object::SetShape(Shape* shape) {
    m_Shape = shape;
}

Shape* Object::GetShape() {
    return m_Shape;
}

void Object::SetMaterial(Material* material) {
    m_Material = material;
}

Material* Object::GetMaterial() {
    return m_Material;
}

bool Object::Hit(const Ray& r, Vector3& p, Vector3& n, float& t) {
    if (m_Shape == 0) return false;

    return m_Shape->Hit(r, p, n, t);
}

};  // namespace spt