#include "plane.hpp"
#include <OpenGL/gl.h>
#include "../raytracing_stats.hpp"

const float INF = 99999;

bool Plane::intersect(const Ray& r, Hit& h, float tmin)
{
    RayTracingStats::IncrementNumIntersections(); 

    float t = -(-d + normal.Dot3(r.getOrigin())) / normal.Dot3(r.getDirection());
    if (t > tmin && t < h.getT())
    {
        h.set(t, material, normal, r);
        return true;
    }
    return false;
}

void Plane::paint()
{
    material->glSetMaterial();
 
    Vec3f v = (normal == Vec3f(1, 0, 0)) ? Vec3f(0, 1, 0) : Vec3f(1, 0, 0);
    Vec3f b1, b2, central;
    Vec3f::Cross3(b1, normal, v);
    Vec3f::Cross3(b2, normal, b1);
    if (normal.z() != 0) { central = Vec3f(0, 0, d / normal.z()); }
	else if (normal.y() != 0) {	central = Vec3f(0, d / normal.y(), 0); }
	else { central = Vec3f(d / normal.x(), 0, 0); }
 
    Vec3f vex[4] = { central + b1 * INF, central + b2 * INF,central - b1 * INF,central - b2 * INF };
 
    glBegin(GL_QUADS);
    glNormal3f(normal.x(), normal.y(), normal.z());
    for (int i = 0; i < 4; i++)
    {
        glVertex3f(vex[i].x(), vex[i].y(), vex[i].z());
    }
    glEnd();
}

void Plane::insertIntoGrid(Grid* g, Matrix* m)
{
    if(m != NULL)
    {
        return;
    }
    g->insertNonInGridObject(this);
}