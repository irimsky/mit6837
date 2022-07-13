#ifndef _OBJECT3D_H
#define _OBJECT3D_H

#include <iostream>
using namespace std;

#include "../ray.hpp"
#include "../hit.hpp"
#include "../material.hpp"
#include "../boundingbox.hpp"

class Grid;

class Object3D
{
public:
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    virtual void paint() = 0;
    virtual void insertIntoGrid(Grid *g, Matrix *m) { }

    Object3D() { cout << "obj3d init" << endl; material = NULL; }
    ~Object3D() { delete material; }

    Material* getMaterial()
    {
        return material;
    }
    BoundingBox* getBoundingBox(){ return boundingBox; };

protected:
    Material *material;
    BoundingBox *boundingBox;
};

#endif