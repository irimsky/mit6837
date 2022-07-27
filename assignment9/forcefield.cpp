#include "forcefield.h"

const float G = 9.8;

Vec3f GravityForceField::getAcceleration(const Vec3f &position, float mass, float t) const
{
    return gravity;
}

Vec3f ConstantForceField::getAcceleration(const Vec3f &position, float mass, float t) const
{
    Vec3f res(force * 1.0f);
    res /= mass;
    return res;
}

Vec3f RadialForceField::getAcceleration(const Vec3f &position, float mass, float t) const
{
    return position * ( -magnitude);
}

Vec3f VerticalForceField::getAcceleration(const Vec3f &position, float mass, float t) const
{
    return Vec3f(0, position.y(), 0) * (-magnitude);
}