#ifndef _SPLINE_H
#define _SPLINE_H

#include "arg_parser.hpp"
#include "vectors.hpp"
#include "triangle_mesh.hpp"

class Spline
{
public:
    Spline(){};
    ~Spline(){};

    // FOR VISUALIZATION
    virtual void Paint(ArgParser *args) {};

    // FOR CONVERTING BETWEEN SPLINE TYPES
    virtual void OutputBezier(FILE *file) = 0;
    virtual void OutputBSpline(FILE *file) = 0;

    // FOR CONTROL POINT PICKING
    virtual int getNumVertices() { return 0; };
    virtual Vec3f getVertex(int i) { return Vec3f();};

    // FOR EDITING OPERATIONS
    virtual void moveControlPoint(int selectedPoint, float x, float y) {};
    virtual void addControlPoint(int selectedPoint, float x, float y) {};
    virtual void deleteControlPoint(int selectedPoint) {};

    // FOR GENERATING TRIANGLES
    virtual TriangleMesh* OutputTriangles(ArgParser *args) { return NULL; };
};


#endif