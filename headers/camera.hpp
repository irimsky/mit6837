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
    
    virtual void glInit(int w, int h) = 0;
    virtual void glPlaceCamera(void) = 0;
    virtual void dollyCamera(float dist) = 0;
    virtual void truckCamera(float dx, float dy) = 0;
    virtual void rotateCamera(float rx, float ry) = 0;

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
    virtual void glInit(int w, int h);
    virtual void glPlaceCamera(void);
    virtual void dollyCamera(float dist);
    virtual void truckCamera(float dx, float dy);
    virtual void rotateCamera(float rx, float ry);

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
public:
    PerspectiveCamera(Vec3f& center, Vec3f& direction, Vec3f& up, float angle)
    {
        this->angle = angle;
        this->center = center;
        this->direction = direction; this->direction.Normalize();
        this->up = up; this->up.Normalize();
    }

    ~PerspectiveCamera();

    virtual Ray generateRay(Vec2f point);

    virtual void glInit(int w, int h);
    virtual void glPlaceCamera(void);
    virtual void dollyCamera(float dist);
    virtual void truckCamera(float dx, float dy);
    virtual void rotateCamera(float rx, float ry);

    virtual float getTMin() const
    {
        return 0;
    }
    
private:
    Vec3f center;
    Vec3f direction;
    Vec3f up;
    float angle;
};

#endif