#include "group.hpp"
#include "grid.hpp"
#include <iostream>
#include <assert.h>

using namespace std;

bool Group::intersect(const Ray &r, Hit &h, float tmin)
{
    bool flag = false;
    for(Object3D* objPtr:objPtrs)
    {
        if(objPtr->intersect(r, h, tmin))
        {
            flag = true;
        }
    }
    return flag;
}

void Group::addObject(int index, Object3D *obj)
{
    assert(index >= 0 && index < objPtrs.size() && !objPtrs[index]);
    objPtrs[index] = obj;
    
    if(boundingBox == NULL)
    {
        boundingBox = new BoundingBox(obj->getBoundingBox()->getMin(), obj->getBoundingBox()->getMax());
        boundingBox->Print();
    }
    else
    {
        boundingBox->Print();
        if(obj->getBoundingBox() != NULL)
            boundingBox->Extend(obj->getBoundingBox());
    }

}


void Group::paint()
{
    for(Object3D* objPtr:objPtrs)
    {
        objPtr->paint();
    }
}

void Group::insertIntoGrid(Grid *g, Matrix *m)
{
    for(Object3D* objPrt:objPtrs)
    {
        objPrt->insertIntoGrid(g, m);
    }
}
