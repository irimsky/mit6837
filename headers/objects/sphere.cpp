
#include "sphere.hpp"

bool Sphere::intersect(const Ray &r, Hit &h, float tmin)
{
    float a = r.getDirection().Dot3(r.getDirection());
    Vec3f tmp = r.getOrigin() - center;
    float b = 2 * tmp.Dot3(r.getDirection());
    float c = tmp.Dot3(tmp) - radius * radius;
    //std::cout << a << ' ' << b << ' ' << c << std::endl;
    float invA = 1.0f / a;
    float t0, t1;
    float discr = b * b - 4 * a * c;
    // if(discr > -5)
    //     std::cout << discr << std::endl;
    if (discr < 0)
        return false;
    else if (discr == 0)
        t0 = t1 = -0.5 * b * invA;
    else
    {
        float q = (b > 0) ? -0.5 * (b + sqrt(discr)) : -0.5 * (b - sqrt(discr));
        t0 = q * invA;
        t1 = c / q;
    }
    if (t0 > t1)
        std::swap(t0, t1);

    //std::cout << t0 << ' ' << t1 << std::endl;
    float t;

    if(t0 >= tmin) t = t0;
    else if(t1 >= tmin) t = t1;
    else return false;

    //std::cout << t << std::endl;

    if (t < h.getT()){
        Vec3f n = r.pointAtParameter(t) - center;
        n.Normalize();
        h.set(t, this->material, n, r);
        //std::cout << "update Hit" << std::endl;
        //std::cout << h << std::endl;
        return true;
    }
    return false;
}