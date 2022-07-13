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
    virtual void paint();
    virtual void insertIntoGrid(Grid* g, Matrix* m);
    
    Transform(Matrix& m, Object3D* o)
    {
        cout << "transform init" << endl;
        mat = m;
        obj = o;
        calBoundingBox();
    }

    void calBoundingBox()
    {
        Vec3f bmin = obj->getBoundingBox()->getMin(), bmax = obj->getBoundingBox()->getMax();
        mat.Transform(bmin); mat.Transform(bmax);
        boundingBox = new BoundingBox(bmin, bmax);
    }

    ~Transform() { delete boundingBox; };
};


#endif