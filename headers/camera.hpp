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

    Vec3f getCenter()
    {
        return this->center;
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

class PerspectiveCamera : public Camera
{
    // for a perspective camera, the field of view is specified with an angle
    float angle;
public:
    PerspectiveCamera(Vec3f& center, Vec3f& direction, Vec3f& up, float angle)
    {
        fov = angle;
        this->center = center;
        this->direction = direction; this->direction.Normalize();
        this->up = up; this->up.Normalize();
    }

    ~PerspectiveCamera();

    virtual Ray generateRay(Vec2f point);

    virtual float getTMin() const
    {
        return 0;
    }
    
private:
    Vec3f center;
    Vec3f direction;
    Vec3f up;
    float fov;
};

#endif