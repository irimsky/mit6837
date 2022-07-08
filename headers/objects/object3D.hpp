#ifndef _OBJECT3D_H
#define _OBJECT3D_H

#include <iostream>
using namespace std;

#include "../ray.hpp"
#include "../hit.hpp"
#include "../material.hpp"

class Object3D
{
public:
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    Object3D() { material = NULL; }
    ~Object3D() { delete material; }

    Material* getMaterial()
    {
        return material;
    }

protected:
    Material *material;
};

#endif