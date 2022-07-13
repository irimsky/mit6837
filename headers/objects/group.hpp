#ifndef _GROUP_H
#define _GROUP_H

#include <iostream>
#include <vector>
using namespace std;

#include "object3D.hpp"
#include "group.hpp"
#include "grid.hpp"

class Group : public Object3D
{
public:
    virtual bool intersect(const Ray &r, Hit &h, float tmin);
    virtual void paint();
    virtual void insertIntoGrid(Grid *g, Matrix *m);

    void addObject(int index, Object3D *obj);

    void calBoundingBox()
    {
        boundingBox = new BoundingBox(objPtrs[0]->getBoundingBox()->getMin(), objPtrs[0]->getBoundingBox()->getMax());
        for (Object3D *objPtr : objPtrs)
        {
            if(objPtr->getBoundingBox() != NULL)
                boundingBox->Extend(objPtr->getBoundingBox());
        }
    }

    Group()
    {
        boundingBox = NULL;
    }
    Group(uint num)
    {
        objPtrs.resize(num, NULL);
        boundingBox = NULL;
    }

    ~Group() { delete boundingBox; }

protected:
    vector<Object3D *> objPtrs;
};

#endif