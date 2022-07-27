#ifndef _FORCEFILED_H
#define _FORCEFILED_H

#include "vectors.h"

class ForceField
{
public:
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const = 0;
};

class GravityForceField : public ForceField
{
public:
    Vec3f gravity;

    GravityForceField(Vec3f gravity)
    {
        this->gravity = gravity;
    }

    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const;
};

class ConstantForceField : public ForceField
{
public:
    Vec3f force;

    ConstantForceField(Vec3f force)
    {
        this->force = force;
    }

    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const;
};

class RadialForceField : public ForceField
{
public:
    float magnitude;
    RadialForceField(float magnitude)
    {
        this->magnitude = magnitude;
    }
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const;
};

class VerticalForceField : public ForceField
{
public:
    float magnitude;
    VerticalForceField(float magnitude)
    {
        this->magnitude = magnitude;
    }
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const;
};

#endif