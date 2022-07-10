#include "plane.hpp"


bool Plane::intersect(const Ray& r, Hit& h, float tmin)
{
    float t = -(-d + normal.Dot3(r.getOrigin())) / normal.Dot3(r.getDirection());
    if (t > tmin && t < h.getT())
    {
        h.set(t, material, normal, r);
        return true;
    }
    return false;
}
