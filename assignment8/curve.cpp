#include "curve.hpp"

#include "matrix.hpp"

#include <OpenGL/gl.h>
//#include <GL/glew.h>
#include <OpenGL/glu.h>
//#include <GLFW/glfw3.h>
#include <GLUT/glut.h>

void Curve::Paint(ArgParser *args)
{
    glColor3f(0.f, 0.f, 1.f);
    glLineWidth(1);
    glBegin(GL_LINES);
    for (int i = 1; i < len; i++)
    {
        glVertex3f(vertices[i - 1].x(), vertices[i - 1].y(), vertices[i - 1].z());
        glVertex3f(vertices[i].x(), vertices[i].y(), vertices[i].z());
    }
    glEnd();

    glColor3f(1.f, 1.f, 1.f);
    glPointSize(5);
    glBegin(GL_POINTS);
    for (int i = 0; i < len; i++)
    {
        glVertex3f(vertices[i].x(), vertices[i].y(), vertices[i].z());
    }
    glEnd();

    glColor3f(0.f, 1.f, 0.f);
    glLineWidth(1);
    glBegin(GL_LINES);
    bool start = true;
    vector<Vec3f> vertices = getPoints(args->curve_tessellation);
    Vec3f m;
    for (int i = 0; i < vertices.size(); i++)
    {
        m = vertices[i];
        if (start)
        {
            start = false;
            glVertex3f(m.x(), m.y(), m.z());
        }
        else
        {
            glVertex3f(m.x(), m.y(), m.z());
            glVertex3f(m.x(), m.y(), m.z());
        }
    }
    glVertex3f(m.x(), m.y(), m.z());
    glEnd();
}

static const float f[] = {-1, 3, -3, 1, 3, -6, 3, 0, -3, 3, 0, 0, 1, 0, 0, 0};
static const float f2[] = {-1.0 / 6, 3.0 / 6, -3.0 / 6, 1.0 / 6, 3.0 / 6, -6.0 / 6, 0.0 / 6, 4.0 / 6,
                           -3.0 / 6, 3.0 / 6, 3.0 / 6, 1.0 / 6, 1.0 / 6, 0.0 / 6, 0.0 / 6, 0.0 / 6};

Vec3f BezierCurve::getPos(int l, float t)
{
    static const float f[] = {-1, 3, -3, 1, 3, -6, 3, 0, -3, 3, 0, 0, 1, 0, 0, 0};
    static const Matrix m(f);
    Vec4f TVector(t * t * t, t * t, t, 1);
    m.Transform(TVector);
    return vertices[l] * TVector.x() + vertices[l + 1] * TVector.y() + vertices[l + 2] * TVector.z() + vertices[l + 3] * TVector.w();
}

void BezierCurve::OutputBezier(FILE *file)
{
    cout << "be::be" << endl;
    fprintf(file, "bezier\n");
    fprintf(file, "num_vertices %d\n", getNumVertices());
    for (int i = 0; i < getNumVertices(); i++)
    {
        Vec3f point = getVertex(i);
        fprintf(file, "%f %f %f\n", point[0], point[1], point[2]);
    }
}

void BezierCurve::OutputBSpline(FILE *file)
{
    cout << "be::bs" << endl;
    static const Matrix Bezier_Matrix(f);
    static const Matrix BSpline_Matrix(f2);

    fprintf(file, "bspline\n");
    fprintf(file, "num_vertices %d\n", int((getNumVertices() - 1) / 3 + 3));
    Matrix ma_BS = Matrix(BSpline_Matrix);
    ma_BS.Inverse();
    for (int i = 0; i <= getNumVertices() - 3; i += 3)
    {
        Vec3f p1 = getVertex(i);
        Vec3f p2 = getVertex(i + 1);
        Vec3f p3 = getVertex(i + 2);
        Vec3f p4 = getVertex(i + 3);
        float coffi[16] = {p1[0], p2[0], p3[0], p4[0], p1[1], p2[1], p3[1], p4[1], p1[2], p2[2], p3[2], p4[2], 0, 0, 0, 0};
        Matrix Geogetry = Matrix(coffi);
        Matrix res = Geogetry * (Bezier_Matrix * ma_BS);
        // res.ShowMatrix();
        if (i == 0)
        {
            fprintf(file, "%f %f %f\n", res.Get(0, 0), res.Get(0, 1), res.Get(0, 2));
            fprintf(file, "%f %f %f\n", res.Get(1, 0), res.Get(1, 1), res.Get(1, 2));
            fprintf(file, "%f %f %f\n", res.Get(2, 0), res.Get(2, 1), res.Get(2, 2));
        }
        fprintf(file, "%f %f %f\n", res.Get(3, 0), res.Get(3, 1), res.Get(3, 2));
    }
}

TriangleMesh* BezierCurve::OutputTriangles(ArgParser* args)
{
    static const Matrix Bezier_Matrix(f);
    static const Matrix BSpline_Matrix(f2);
	int curve_tess = args->curve_tessellation;
	float curve_step_size = 1.0 / curve_tess;
	int revo_tess = args->revolution_tessellation;
	float revo_step_size = 2.0 * M_PI / revo_tess;

	TriangleNet* TN = new TriangleNet(revo_tess, (getNumVertices()-1) / 3 * curve_tess);
	for (int i = 0; i <= getNumVertices()-3; i += 3)
	{
		Vec4f PowerBasis = Vec4f(0, 0, 0, 1);
		Vec3f p1 = getVertex(i);
		Vec3f p2 = getVertex(i + 1);
		Vec3f p3 = getVertex(i + 2);
		Vec3f p4 = getVertex(i + 3);
		float coffi[16] = { p1[0],p2[0],p3[0],p4[0],p1[1],p2[1],p3[1],p4[1],p1[2],p2[2],p3[2],p4[2],0,0,0,0 };
		Matrix Geogetry = Matrix(coffi);
		for (int j = 0; j <= curve_tess; j++)
		{
			float t = PowerBasis[2] + curve_step_size;
			Bezier_Matrix.Transform(PowerBasis);
			Geogetry.Transform(PowerBasis);
			float theta = 0;
			for (int k = 0; k <= revo_tess; k++)
			{
				float x = cos(theta) * PowerBasis[0] + sin(theta) * PowerBasis[2];
				float z = -sin(theta) * PowerBasis[0] + cos(theta) * PowerBasis[2];
				TN->SetVertex(k, j + (i / 3 * curve_tess), Vec3f(x, PowerBasis[1], z));
				theta += revo_step_size;
			}
			PowerBasis.Set(t * t * t, t * t, t, 1);

		}
	}
	return TN;
}

Vec3f BSplineCurve::getPos(int l, float t)
{
    static const float f[] = {-1, 3, -3, 1, 3, -6, 0, 4, -3, 3, 3, 1, 1, 0, 0, 0};
    static const Matrix m(f);
    Vec4f TVector(t * t * t, t * t, t, 1);
    TVector *= (1.f / 6);
    m.Transform(TVector);
    return vertices[l] * TVector.x() + vertices[l + 1] * TVector.y() + vertices[l + 2] * TVector.z() + vertices[l + 3] * TVector.w();
}

void BSplineCurve::OutputBezier(FILE *file)
{
    static const Matrix Bezier_Matrix(f);
    static const Matrix BSpline_Matrix(f2);

    cout << "bs::be" << endl;
    fprintf(file, "bezier\n");
    fprintf(file, "num_vertices %d\n", (getNumVertices() - 3) * 3 + 1);
    Matrix ma_BS = Matrix(Bezier_Matrix);
    ma_BS.Inverse();
    for (int i = 0; i < getNumVertices() - 3; i++)
    {
        Vec3f p1 = getVertex(i);
        Vec3f p2 = getVertex(i + 1);
        Vec3f p3 = getVertex(i + 2);
        Vec3f p4 = getVertex(i + 3);
        float coffi[16] = {p1[0], p2[0], p3[0], p4[0], p1[1], p2[1], p3[1], p4[1], p1[2], p2[2], p3[2], p4[2], 0, 0, 0, 0};
        Matrix Geogetry = Matrix(coffi);
        Matrix res = Geogetry * BSpline_Matrix * ma_BS;
        if (i == 0)
            fprintf(file, "%f %f %f\n", res.Get(0, 0), res.Get(0, 1), res.Get(0, 2));
        fprintf(file, "%f %f %f\n", res.Get(1, 0), res.Get(1, 1), res.Get(1, 2));
        fprintf(file, "%f %f %f\n", res.Get(2, 0), res.Get(2, 1), res.Get(2, 2));
        fprintf(file, "%f %f %f\n", res.Get(3, 0), res.Get(3, 1), res.Get(3, 2));
    }
}

void BSplineCurve::OutputBSpline(FILE *file)
{
    cout << "bs::bs" << endl;
    fprintf(file, "bspline\n");
    fprintf(file, "num_vertices %d\n", getNumVertices());
    for (int i = 0; i < getNumVertices(); i++)
    {
        Vec3f point = getVertex(i);
        fprintf(file, "%f %f %f\n", point[0], point[1], point[2]);
    }
}

TriangleMesh* BSplineCurve::OutputTriangles(ArgParser* args)
{
    static const Matrix Bezier_Matrix(f);
    static const Matrix BSpline_Matrix(f2);
	int curve_tess = args->curve_tessellation;
	float curve_step_size = 1.0 / curve_tess;
	int revo_tess = args->revolution_tessellation;
	float revo_step_size = 2.0 * M_PI / revo_tess;

	TriangleNet* TN = new TriangleNet(revo_tess, (getNumVertices() - 3) * curve_tess);

	for (int i = 0; i < getNumVertices()-3; i++)
	{
		Vec4f PowerBasis = Vec4f(0, 0, 0, 1);
		Vec3f p1 = getVertex(i);
		Vec3f p2 = getVertex(i + 1);
		Vec3f p3 = getVertex(i + 2);
		Vec3f p4 = getVertex(i + 3);
		float coffi[16] = { p1[0],p2[0],p3[0],p4[0],p1[1],p2[1],p3[1],p4[1],p1[2],p2[2],p3[2],p4[2],0,0,0,0 };
		Matrix Geogetry = Matrix(coffi);
		for (int j = 0; j <= curve_tess; j++)
		{
			float t = PowerBasis[2] + curve_step_size;
			BSpline_Matrix.Transform(PowerBasis);
			Geogetry.Transform(PowerBasis);
			float theta = 0;
			for (int k = 0; k <= revo_tess; k++)
			{
				float x = cos(theta) * PowerBasis[0] + sin(theta) * PowerBasis[2];
				float z = -sin(theta) * PowerBasis[0] + cos(theta) * PowerBasis[2];
				TN->SetVertex(k, j + (i * curve_tess), Vec3f(x, PowerBasis[1], z));
				theta += revo_step_size;
			}
			PowerBasis.Set(t * t * t, t * t, t, 1);
		}
	}
	return TN;
}