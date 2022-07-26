#ifndef _CURVE_H
#define _CURVE_H

#include "arg_parser.hpp"
#include "vectors.hpp"
#include "triangle_mesh.hpp"
#include "spline.hpp"
#include "matrix.hpp"
#include <vector>

// TODO9
class Curve : public Spline
{
protected:
    int len;
    vector<Vec3f> vertices;

public:
    Curve() {}
    Curve(int l)
    {
        len = l;
        vertices.resize(l);
    }
    ~Curve(){}
    virtual void Paint(ArgParser *args);

    void set(int i, Vec3f v)
    {
        vertices[i] = v;
    }

    virtual Vec3f getPos(int l, float t) = 0;
    virtual void OutputBezier(FILE *file)=0;//{ cout << "curve out Be" << endl; }
    virtual void OutputBSpline(FILE *file)=0;//{ cout << "curve out Bs" << endl; }

    Vec3f getVertex(int i)
    {
        return vertices[i];
    };
    virtual int getNumVertices()
    {
        return len;
    };
    virtual void moveControlPoint(int selectedPoint, float x, float y)
    {
        vertices[selectedPoint].Set(x, y, 0);
    };
    virtual void addControlPoint(int selectedPoint, float x, float y)
    {
        vertices.insert(vertices.begin() + selectedPoint, Vec3f(x, y, 0));
        len++;
    };
    virtual void deleteControlPoint(int selectedPoint)
    {
        vertices.erase(vertices.begin() + selectedPoint);
        len--;
    };
    virtual vector<Vec3f> getPoints(int tessellation) = 0;
};

class BezierCurve : public Curve
{
public:
    static Vec3f getPos(Vec3f v1, Vec3f v2, Vec3f v3, Vec3f v4, float t)
    {
        static const float f[] = {-1, 3, -3, 1, 3, -6, 3, 0, -3, 3, 0, 0, 1, 0, 0, 0};
        static const Matrix m(f);
        Vec4f TVector(t * t * t, t * t, t, 1);
        m.Transform(TVector);
        return v1 * TVector.x() + v2 * TVector.y() + v3 * TVector.z() + v4 * TVector.w();
    }

    virtual Vec3f getPos(int l, float t);
    virtual void OutputBezier(FILE *file);
    virtual void OutputBSpline(FILE *file);
    virtual TriangleMesh* OutputTriangles(ArgParser* args);


    BezierCurve() {}
    BezierCurve(int l)
    {
        len = l;
        vertices.resize(l);
    }
    ~BezierCurve() {}
    virtual vector<Vec3f> getPoints(int tessellation)
    {
        vector<Vec3f> result;
        float delta_t = 1. / tessellation;

        for (int i = 0; i < len - 3; i += 3)
        {
            for (int j = 0; j <= tessellation; j++)
            {
                Vec3f m = getPos(i, delta_t * j);
                result.push_back(m);
            }
        }
        return result;
    }
};

class BSplineCurve : public Curve
{
public:
    virtual Vec3f getPos(int l, float t);
    virtual void OutputBezier(FILE *file);
    virtual void OutputBSpline(FILE *file);
    virtual TriangleMesh* OutputTriangles(ArgParser* args);

    BSplineCurve() {}
    BSplineCurve(int l)
    {
        len = l;
        vertices.resize(l);
    }
    ~BSplineCurve() {}
    virtual vector<Vec3f> getPoints(int tessellation)
    {
        vector<Vec3f> result;
        float delta_t = 1. / tessellation;

        for (int i = 0; i < len - 3; i++)
        {
            for (int j = 0; j <= tessellation; j++)
            {
                Vec3f m = getPos(i, delta_t * j);
                result.push_back(m);
            }
        }
        return result;
    }
};

#endif