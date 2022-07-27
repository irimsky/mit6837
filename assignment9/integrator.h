#ifndef _INTEGRATOR_H
#define _INTEGRATOR_H

#include "particle.h"
#include "forcefield.h"

class Integrator {
public:
    virtual Vec3f getColor() { return Vec3f(1.0, 1.0, 1.0); }
    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt) = 0;
};


class EulerIntegrator : public Integrator {
public:
    virtual Vec3f getColor() { return Vec3f(1.0, 0, 0); }
    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt);
};

class MidpointIntegrator : public Integrator {
public:
    virtual Vec3f getColor() { return Vec3f(0, 1.0, 0); }
    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt);
};


#endif
