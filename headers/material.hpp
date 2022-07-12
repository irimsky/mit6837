#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.hpp"
#include "ray.hpp"
#include "hit.hpp"
// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Ray;
class Hit;

class Material
{

public:
    // CONSTRUCTORS & DESTRUCTOR
    Material(const Vec3f &d_color) { diffuseColor = d_color; }
    Material() { diffuseColor = Vec3f(0, 0, 0); }
    virtual ~Material() {}

    // ACCESSORS
    Vec3f getDiffuseColor() const { return diffuseColor; }
    Vec3f getSpecularColor() const { return specularColor; }
    Vec3f getTransparentColor() const { return transparentColor; }
    Vec3f getReflectiveColor() const { return reflectiveColor; }
    float getIndexOfRefraction() const { return indexOfRefraction; }

    virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const = 0;



    virtual void glSetMaterial(void) const = 0;


protected:
    // REPRESENTATION
    Vec3f diffuseColor;
    Vec3f specularColor;
    float exponent;
    Vec3f reflectiveColor;
    Vec3f transparentColor;
    float indexOfRefraction;

    

};

// ====================================================================
// ====================================================================

class PhongMaterial : public Material
{
protected:
    
public:
    PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, float exponent)
    {
        this->diffuseColor = diffuseColor;
        this->specularColor = specularColor;
        this->exponent = exponent;
    }

    PhongMaterial(const Vec3f &diffuseColor,
                                 const Vec3f &specularColor,
                                 float exponent,
                                 const Vec3f &reflectiveColor,
                                 const Vec3f &transparentColor,
                                 float indexOfRefraction)

    {
        this->diffuseColor = diffuseColor;
        this->specularColor = specularColor;
        this->exponent = exponent;
        this->reflectiveColor = reflectiveColor;
        this->transparentColor = transparentColor;
        this->indexOfRefraction = indexOfRefraction;
    }

    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;
    void glSetMaterial() const;
    
};

#endif