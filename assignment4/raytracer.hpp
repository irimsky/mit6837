#ifndef _RAYTRACER_H
#define _RAYTRACER_H

#include "../headers/vectors.hpp"
#include "../headers/ray.hpp"
#include "../headers/hit.hpp"
#include "../headers/global.hpp"
#include "../headers/objects/group.hpp"
#include "scene_parser.hpp"

class SceneParser;

class RayTracer
{
private:
    SceneParser* s;

public:
    RayTracer() {}
    ~RayTracer() {}

    RayTracer(SceneParser *s)
    {
        this->s = s;
    }

    Vec3f traceRay(Ray &ray, float tmin,
                   int bounces, float weight,
                   float indexOfRefraction, Hit &hit) const;
};

#endif