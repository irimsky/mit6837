#ifndef _CAMERA_H
#define _CAMERA_H

#include <iostream>
using namespace std;

#include "ray.hpp"
#include "hit.hpp"

class Camera
{
public:
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;

    Camera() { }
    ~Camera() { }
};


class OrthographicCamera : public Camera{
public:
    OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size)
    {
        this->center = center;
        this->direction = direction; this->direction.Normalize();
        this->up = up; this->up.Normalize();
        this->size = size;
    }

    virtual Ray generateRay(Vec2f point);

    virtual float getTMin() const 
    {
        return -9999999.0f;
    }

private:
    Vec3f center;
    Vec3f direction;
    Vec3f up;
    float size;
};

#endif