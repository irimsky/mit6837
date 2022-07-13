#ifndef _SPHERE_H
#define _SPHERE_H

#include <iostream>
using namespace std;

#include "object3D.hpp"
#include "grid.hpp"

class Sphere : public Object3D
{
public:
    virtual bool intersect(const Ray &r, Hit &h, float tmin);
    virtual void paint();
    virtual void insertIntoGrid(Grid *g, Matrix *m);

    Sphere()
    {
        cout << "sphere empty init" << endl;
        radius = 0;
        center = Vec3f(0, 0, 0);
        calBoundingBox();
    }

    Sphere(Vec3f c, float r, Material *m)
    {
        cout << "sphere init" << endl;
        radius = r;
        center = c;
        material = m;
        calBoundingBox();
        cout << "sphere init end" << endl;
    }

    void calBoundingBox()
    {
        boundingBox = new BoundingBox(center - Vec3f(radius, radius, radius), center + Vec3f(radius, radius, radius));
    }

    ~Sphere() { delete boundingBox; }

protected:
    Vec3f center;
    float radius;
};

#endif