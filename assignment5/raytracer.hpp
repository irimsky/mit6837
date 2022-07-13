#ifndef _RAYTRACER_H
#define _RAYTRACER_H

#include "../headers/vectors.hpp"
#include "../headers/ray.hpp"
#include "../headers/hit.hpp"
#include "../headers/global.hpp"
#include "../headers/objects/group.hpp"
#include "../headers/objects/grid.hpp"
#include "scene_parser.hpp"

class SceneParser;

class RayTracer
{
private:
    SceneParser* s;
    Grid* grid;

public:
    RayTracer() {}
    ~RayTracer() {}

    RayTracer(SceneParser *s, Grid* grid = NULL)
    {
        this->s = s;
        this->grid = grid;
    }

    Vec3f traceRay(Ray &ray, float tmin,
                   int bounces, float weight,
                   float indexOfRefraction, Hit &hit) const;
};

#endif