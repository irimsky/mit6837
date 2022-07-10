#ifndef _PLANE_H
#define _PLANE_H

#include <iostream>
using namespace std;

#include "object3D.hpp"


class Plane : public Object3D
{
public:
    virtual bool intersect(const Ray &r, Hit &h, float tmin);
    Plane(Vec3f& normal, float d, Material* m)
    {
        this->normal = normal;
        this->d = d;
        this->material = m;
    }
    ~Plane();
private:
    Vec3f normal;
    float d;
};

#endif