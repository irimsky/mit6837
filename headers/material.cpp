#include "material.hpp"
#include "ray.hpp"
#include "hit.hpp"
#include "matrix.hpp"
#include "perlin_noise.hpp"
#include <iostream>
#include <math.h>
#include <OpenGL/gl.h>



Vec3f PhongMaterial::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const
{
    // for directional light sources, dirToLight is normalized
    // coefficient is not set yet
    Vec3f dirlightnormal = dirToLight;
    dirlightnormal.Normalize();

    // Diffuse term
    float t = dirlightnormal.Dot3(hit.getNormal());
    if (t < 0)
        return Vec3f(0, 0, 0);
    Vec3f Diffuse = t * getDiffuseColor();

    // Specular term
    Vec3f raydire = ray.getDirection();
    raydire.Normalize();
    Vec3f Specular;

    Vec3f r = dirlightnormal - raydire;
    r.Normalize();
    Specular = pow(std::max(hit.getNormal().Dot3(r), (float)0), exponent) * getSpecularColor();

    Vec3f result = (Diffuse + Specular) * lightColor;
    if (result.x() != 0)
        return result;
    return result;
}

void PhongMaterial::glSetMaterial() const
{

    GLfloat one[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat zero[4] = {0.0, 0.0, 0.0, 0.0};
    GLfloat specular[4] = {
        getSpecularColor().r(),
        getSpecularColor().g(),
        getSpecularColor().b(),
        1.0};
    GLfloat diffuse[4] = {
        getDiffuseColor().r(),
        getDiffuseColor().g(),
        getDiffuseColor().b(),
        1.0};

    // NOTE: GL uses the Blinn Torrance version of Phong...
    float glexponent = exponent;
    if (glexponent < 0)
        glexponent = 0;
    if (glexponent > 128)
        glexponent = 128;

#if !SPECULAR_FIX

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

#else

    // OPTIONAL: 3 pass rendering to fix the specular highlight
    // artifact for small specular exponents (wide specular lobe)

    if (SPECULAR_FIX_WHICH_PASS == 0)
    {
        // First pass, draw only the specular highlights
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
    }
    else if (SPECULAR_FIX_WHICH_PASS == 1)
    {
        // Second pass, compute normal dot light
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
    }
    else
    {
        // Third pass, add ambient & diffuse terms
        assert(SPECULAR_FIX_WHICH_PASS == 2);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
    }

#endif
}

Vec3f Checkerboard::Shade(const Ray & ray, const Hit & hit, const Vec3f & dirToLight, const Vec3f & lightColor) const
{
    Vec3f intersection = hit.getIntersectionPoint();
    mat->Transform(intersection);
    bool whichmaterial = int(round(intersection[0]) + round(intersection[1]) + round(intersection[2])) & 1;
    if (whichmaterial)
    {
        return material1->Shade(ray, hit, dirToLight, lightColor);
    }
    else
    {
        return material2->Shade(ray, hit, dirToLight, lightColor);
    }
    return Vec3f();
}

void Checkerboard::glSetMaterial(void) const
{
    material1->glSetMaterial();
}

float Noise::NoiseCalculate(Vec3f intersection, int octaves)
{
    float c = 0;
    float times = 1.0;
    for (int i = 0; i < octaves; i++)
    {
        c += PerlinNoise::noise(intersection.x(), intersection.y(), intersection.z()) * times;
        times /= 2.0;
        intersection *= 2.0f;
    }
    return c;
}

Vec3f Noise::Shade(const Ray & ray, const Hit & hit, const Vec3f & dirToLight, const Vec3f & lightColor) const
{
    Vec3f intersection = hit.getIntersectionPoint();
    mat->Transform(intersection);
    float c = Noise::NoiseCalculate(intersection, octaves);
    Vec3f color1 = material1->Shade(ray, hit, dirToLight, lightColor);
    Vec3f color2 = material2->Shade(ray, hit, dirToLight, lightColor);
    return color1 * (1 - c) + color2 * c;
}

void Noise::glSetMaterial(void) const
{
    material1->glSetMaterial();
}

Vec3f Marble::Shade(const Ray & ray, const Hit & hit, const Vec3f & dirToLight, const Vec3f & lightColor) const
{
    Vec3f intersection = hit.getIntersectionPoint();
    float c = sin(frequency * intersection.x() + amplitude * Noise::NoiseCalculate(intersection, octaves));
    Vec3f color1 = material1->Shade(ray, hit, dirToLight, lightColor);
    Vec3f color2 = material2->Shade(ray, hit, dirToLight, lightColor);
    return color1 * ( 1 - c) + color2 * c;
}

void Marble::glSetMaterial(void) const
{
    material1->glSetMaterial();
}

Vec3f Wood::Shade(const Ray & ray, const Hit & hit, const Vec3f & dirToLight, const Vec3f & lightColor) const
{
    Vec3f intersection = hit.getIntersectionPoint();
    float c = sin(frequency * intersection.x() + amplitude * Noise::NoiseCalculate(intersection, octaves));
    Vec3f color1 = material1->Shade(ray, hit, dirToLight, lightColor);
    Vec3f color2 = material2->Shade(ray, hit, dirToLight, lightColor);
    return color1 * (1 - c) + color2 * c;
}

void Wood::glSetMaterial(void) const
{
    material1->glSetMaterial();
}

