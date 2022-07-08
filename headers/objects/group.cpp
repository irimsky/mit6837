#include "group.hpp"
#include <iostream>
#include <assert.h>

using namespace std;

bool Group::intersect(const Ray &r, Hit &h, float tmin)
{
    bool flag = false;
    for(auto objPtr:objPtrs)
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
}