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

class Checkerboard : public Material
{
    Matrix *mat;
    Material *material1, *material2;
public:
    Checkerboard(Matrix *m, Material *mat1, Material *mat2) : Material(Vec3f(1,1,1)), mat(m), material1(mat1), material2(mat2) {};
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;
    void glSetMaterial(void) const;
    Vec3f getSpecularColor() const { return material1->getSpecularColor(); }
    Vec3f getRedlectiveColor() const { return material1->getReflectiveColor(); }
    Vec3f getTransparentColor() const { return material1->getTransparentColor(); }
    float getIndexOfRef()const { return material1->getIndexOfRefraction(); }
};

class Noise : public Material
{
    int octaves;
    Matrix *mat;
    Material *material1, *material2;
public:
    static float NoiseCalculate(Vec3f pos, int octaves);
    Noise(Matrix *m, Material *mat1, Material *mat2, int _octaves) : Material(Vec3f(1,1,1)), mat(m), material1(mat1), material2(mat2), octaves(_octaves){};
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;
    void glSetMaterial(void) const;
    Vec3f getSpecularColor() const { return material1->getSpecularColor(); }
    Vec3f getRedlectiveColor() const { return material1->getReflectiveColor(); }
    Vec3f getTransparentColor() const { return material1->getTransparentColor(); }
    float getIndexOfRef()const { return material1->getIndexOfRefraction(); }
};

class Marble : public Material
{
    int octaves;
    Matrix *mat;
    Material *material1, *material2;
    float frequency;
    float amplitude;
public:
    Marble(Matrix *m, Material *mat1, Material *mat2, int _octaves, float _frequency, float _amplitude) 
        : Material(Vec3f(1,1,1)), mat(m), material1(mat1), material2(mat2), octaves(_octaves), frequency(_frequency), amplitude(_amplitude) {};
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;
    void glSetMaterial(void) const;
    Vec3f getSpecularColor() const { return material1->getSpecularColor(); }
    Vec3f getRedlectiveColor() const { return material1->getReflectiveColor(); }
    Vec3f getTransparentColor() const { return material1->getTransparentColor(); }
    float getIndexOfRef()const { return material1->getIndexOfRefraction(); }
};


class Wood : public Material
{
    int octaves;
    Matrix *mat;
    Material *material1, *material2;
    float frequency;
    float amplitude;
public:
    Wood(Matrix *m, Material *mat1, Material *mat2, int _octaves, float _frequency, float _amplitude) 
    : Material(Vec3f(1,1,1)), mat(m), material1(mat1), material2(mat2), octaves(_octaves), frequency(_frequency), amplitude(_amplitude) {};
    virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;
    virtual void glSetMaterial(void) const;
};

#endif