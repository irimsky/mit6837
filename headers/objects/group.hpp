#ifndef _GROUP_H
#define _GROUP_H

#include <iostream>
#include <vector>
using namespace std;

#include "object3D.hpp"

class Group : public Object3D {
public:
    virtual bool intersect(const Ray &r, Hit &h, float tmin);
    virtual void paint();

    void addObject(int index, Object3D *obj);


    Group() { }
    Group(uint num) 
    { 
        objPtrs.resize(num, NULL); 
    }

    ~Group() { }

protected:
    vector<Object3D*> objPtrs;
};

#endif