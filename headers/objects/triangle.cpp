#include "triangle.hpp"
#include "grid.hpp"
#include "../matrix.hpp"
#include <OpenGL/gl.h>

#define EPLISON 0.0001

float det3x3(float a1, float a2, float a3,
             float b1, float b2, float b3,
             float c1, float c2, float c3);
 
Triangle::Triangle(Vec3f& ta, Vec3f& tb, Vec3f& tc, Material* m):a(ta), b(tb), c(tc)
{
    cout << "tri init" << endl;
    material = m;
    normal = (tb - ta) * (tc - tb);
    normal.Cross3(normal, tb - ta, tc - tb);
    normal.Normalize();
    calBoundingBox();
}
Triangle::~Triangle() { delete boundingBox; }

void Triangle::calBoundingBox()
{
    boundingBox = new BoundingBox(a, a);
    boundingBox->Extend(b);
    boundingBox->Extend(c);
}

bool Triangle::intersect(const Ray& r, Hit& h, float tmin)
{
    // 重心坐标
    float belta, gamma, t;
    float A;
    A = det3x3(
        a.x() - b.x(), a.x() - c.x(), r.getDirection().x(),
        a.y() - b.y(), a.y() - c.y(), r.getDirection().y(),
        a.z() - b.z(), a.z() - c.z(), r.getDirection().z()
    );
 
    belta = det3x3(
        a.x() - r.getOrigin().x(), a.x() - c.x(), r.getDirection().x(),
        a.y() - r.getOrigin().y(), a.y() - c.y(), r.getDirection().y(),
        a.z() - r.getOrigin().z(), a.z() - c.z(), r.getDirection().z()
    );
    gamma = det3x3(
        a.x() - b.x(), a.x() - r.getOrigin().x(), r.getDirection().x(),
        a.y() - b.y(), a.y() - r.getOrigin().y(), r.getDirection().y(),
        a.z() - b.z(), a.z() - r.getOrigin().z(), r.getDirection().z()
    );
    t = det3x3(
        a.x() - b.x(), a.x() - c.x(), a.x() - r.getOrigin().x(),
        a.y() - b.y(), a.y() - c.y(), a.y() - r.getOrigin().y(),
        a.z() - b.z(), a.z() - c.z(), a.z() - r.getOrigin().z()
    );
    belta /= A;
    gamma /= A;
    t /= A;
 
    if (belta > 1 || belta < 0) return false;
    if (gamma > 1 || gamma < 0) return false;
    if (1 - belta - gamma > 1 || 1 - belta - gamma < 0) return false;

    if (t > tmin && t < h.getT())
    {
        h.set(t, material, normal, r);
        return true;
    }
    return false;
}

void Triangle::paint()
{
    material->glSetMaterial();
    glBegin(GL_TRIANGLES);
    glNormal3f(normal.x(), normal.y(), normal.z());
    glVertex3f(a.x(), a.y(), a.z());
    glVertex3f(b.x(), b.y(), b.z());
    glVertex3f(c.x(), c.y(), c.z());
    //cout << a << ' ' << b << ' ' << c << endl;
    glEnd();
}

void Triangle::insertIntoGrid(Grid* g, Matrix* m)
{
    //g->insertMatrix(this, m);
    // cout << "tri insertIntoGrid" << endl;
    Vec3f vec[3];
    vec[0] = a;
    vec[1] = b;
    vec[2] = c;
    if (m)
    {
        m->Transform(vec[0]);
        m->Transform(vec[1]);
        m->Transform(vec[2]);
    }
    BoundingBox *bb = new BoundingBox(vec[0], vec[0]);
    bb->Extend(vec[1]);
    bb->Extend(vec[2]);

    BoundingBox *gb = g->getBoundingBox();
    Vec3f minimum = bb->getMin() - gb->getMin();
    Vec3f maximum = bb->getMax() - gb->getMin();

    Vec3f block = g->getBoundingBox()->getMax() - g->getBoundingBox()->getMin();
    Vec3f gridSize = g->getGirdSize();
    block.Divide(gridSize.x(), gridSize.y(), gridSize.z());
    minimum.Divide(block.x(), block.y(), block.z());
    maximum.Divide(block.x(), block.y(), block.z());

    maximum += Vec3f(EPLISON, EPLISON, EPLISON);
    minimum -= Vec3f(EPLISON, EPLISON, EPLISON);

    for (int i = max(0.f, (float)floor(minimum.x())); i < min(1.f * gridSize.x(), (float)ceil(maximum.x())); i++)
    {
        for (int j = max(0.f, (float)floor(minimum.y())); j < min(1.f * gridSize.y(), (float)ceil(maximum.y())); j++)
        {
            for (int k = max(0.f, (float)floor(minimum.z())); k < min(1.f * gridSize.z(), (float)ceil(maximum.z())); k++)
            {
                g->insertInto(i, j, k, this);
            }
        }
    }
    delete bb;
}