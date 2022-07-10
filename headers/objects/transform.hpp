#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "object3D.hpp"
#include "../matrix.hpp"


class Transform : public Object3D
{
    Matrix mat;
    Object3D* obj;
public:
    virtual bool intersect(const Ray& r, Hit& h, float tmin);
    Transform(Matrix& m, Object3D* o)
    {
        mat = m;
        obj = o;
    }

    ~Transform();
};


#endif