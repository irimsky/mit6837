#include "transform.hpp"
#include <OpenGL/gl.h>

bool Transform::intersect(const Ray& r, Hit& h, float tmin)
{
    Vec4f newOrigin(r.getOrigin(), 1.0);
    Vec4f newDirection(r.getDirection(), 0.0);
 
    Matrix temp(mat);
    temp.Inverse(temp, 0.001);
    temp.Transform(newOrigin);
    temp.Transform(newDirection);
    
    Vec3f newDirection3(newDirection.x(), newDirection.y(), newDirection.z());
    Vec3f newOrigin3(newOrigin.x(), newOrigin.y(), newOrigin.z());

    if (obj->intersect(Ray(newOrigin3, newDirection3), h, tmin))
    {
        Vec4f newNormal(h.getNormal(), 0.0);
        temp.Transpose();
        temp.Transform(newNormal);
        Vec3f m(newNormal.x(), newNormal.y(), newNormal.z());
        m.Normalize();
        h.set(h.getT(), h.getMaterial(), m, r);
        return true;
    }
    return false;
}

void Transform::paint()
{
    glPushMatrix();
    GLfloat* glMatrix = mat.glGet();
    glMultMatrixf(glMatrix);
    delete[] glMatrix;
 
    obj->paint();
    glPopMatrix();
}