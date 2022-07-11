#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include "object3D.hpp"

class Triangle : public Object3D
{
    Vec3f a, b, c;
    Vec3f normal;
public:
    Triangle(Vec3f& a, Vec3f& b, Vec3f& c, Material* m);
    ~Triangle();
    
    virtual bool intersect(const Ray& r, Hit& h, float tmin);
    virtual void paint();
};

#endif