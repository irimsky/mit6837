
#include "sphere.hpp"
#include "../global.hpp"
#include <OpenGL/gl.h>

const float PI = 3.1415926535;

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

void Sphere::paint(void)
{
    material->glSetMaterial();
    // The number of steps in theta and phi will be controlled by 
    // the command line argument -tessellation <theta-steps> <phi-steps>
    float theta = 2 * PI / theta_steps;
    float phi = PI / phi_steps;
 
    Vec3f* points = new Vec3f[theta_steps * (phi_steps + 1)];
    // initialize all the position for points
    Vec3f top = center + Vec3f(0, 1, 0) * radius;
    Vec3f below = center + Vec3f(0, -1, 0) * radius;
    // generate the appropriate polygons
    // theta should vary between 0 and 360
    // phi must vary between -90 and 90 degrees.
    for (int iPhi = 0; iPhi < phi_steps + 1; iPhi++)
    {
        for (int iTheta = 0; iTheta < theta_steps; iTheta++)
        {
            int index = theta_steps * iPhi + iTheta;
            if (iPhi == 0)
            {
                points[index] = below;
                continue;
            }
            if (iPhi == phi_steps)
            {
                points[index] = top;
                continue;
            }
 
            float tphi = iPhi * phi - PI / 2;
            float ttheta = theta * iTheta;
            points[index] = center + 
                            radius * sin(tphi) * Vec3f(0, 1, 0) + 
                            radius * cos(tphi) * cos(ttheta) * Vec3f(1, 0, 0) + 
                            radius * cos(tphi) * sin(ttheta) * Vec3f(0, 0, 1);
        }
    }
 
    glBegin(GL_QUADS);
    for (int iPhi = 0; iPhi < phi_steps; iPhi++)
    {
        for (int iTheta = 0; iTheta < theta_steps; iTheta++)
        {
            int index[4] = { theta_steps * iPhi + iTheta ,
                             theta_steps * iPhi + (iTheta + 1) % theta_steps,
                             theta_steps * (iPhi + 1) + (iTheta + 1) % theta_steps,
                             theta_steps * (iPhi + 1) + iTheta };
            Vec3f normal;
            if (!gouraud)
            {
                Vec3f::Cross3(normal, points[index[0]] - points[index[1]], points[index[2]] - points[index[1]]);
                if (normal.Length() == 0)
                {
                    Vec3f::Cross3(normal, points[index[1]] - points[index[2]], points[index[3]] - points[index[2]]);
                }
            }
            for (int i = 0; i < 4; i++)
            {
                if (gouraud)
                {
                    normal = points[index[i]] - center;
                }
                normal.Normalize();
                glNormal3f(normal.x(), normal.y(), normal.z());
 
                glVertex3f(points[index[i]].x(), points[index[i]].y(), points[index[i]].z());
            }
        }
    }
    glEnd();
}