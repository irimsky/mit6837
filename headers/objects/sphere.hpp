#ifndef _SPHERE_H
#define _SPHERE_H

#include <iostream>
using namespace std;

#include "object3D.hpp"

class Sphere : public Object3D {
public:
    virtual bool intersect(const Ray &r, Hit &h, float tmin);

    Sphere() { radius = 0; center = Vec3f(0, 0, 0); }
    Sphere(Vec3f c, float r, Material* m) { 
        radius = r; center = c; material = m;
    }
    ~Sphere() { }

protected:
    Vec3f center;
    float radius;
};

#endif