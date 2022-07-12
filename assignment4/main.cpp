#include <iostream>
using namespace std;

#include "../headers/scene_parser.hpp"
#include "../headers/image.hpp"
#include "../headers/camera.hpp"
#include "../headers/objects/group.hpp"
#include "../headers/light.hpp"
#include "glCanvas.hpp"
#include "raytracer.hpp"
#include "../headers/global.hpp"

#include <OpenGL/gl.h>
//#include <GL/glew.h>
#include <OpenGL/glu.h>
//#include <GLFW/glfw3.h>
#include <GLUT/glut.h>

char *input_file = NULL;
int width = 100;
int height = 100;
char *output_file = NULL;
float depth_min = 0;
float depth_max = 1;
char *depth_file = NULL;
char *normal_file = NULL;
bool is_gui;
GLCanvas canvas;
SceneParser* sceneParser;

Vec3f calDirectionalLight(const Hit& hit, Light* light)
{
    Vec3f l, lightCol;
    float distance;
    light->getIllumination(hit.getIntersectionPoint(), l, lightCol, distance);
    // Vec3f v = camera.getCenter() - hit.getIntersectionPoint();
    Vec3f diffuse = hit.getMaterial()->getDiffuseColor();

    return lightCol * diffuse * max(hit.getNormal().Dot3(l), 0.0f);
}

void render()
{
    
    Camera* camera = sceneParser->getCamera();
    Group* group = sceneParser->getGroup();
    Image colorRes(width, height), depthRes(width, height), normalRes(width, height);
    colorRes.SetAllPixels(sceneParser->getBackgroundColor());

    RayTracer rayTracer(sceneParser);

    for(int i=0;i<width;++i)
    {
        for(int j=0;j<height;++j)
        {
            //std::cout << i << ' ' << j << std::endl;
            Ray r = camera->generateRay(Vec2f((i + 0.5f)/width, (j + 0.5f)/height));
            //cout << r << endl;
            Hit h;
            Vec3f color = rayTracer.traceRay(r, camera->getTMin(), 0, 1, 1, h);
            colorRes.SetPixel(j, i, color);
            float d = (h.getT() - depth_min) / (depth_max - depth_min);
            d = max(0.0f, min(1.0f, 1.0f-d));
            depthRes.SetPixel(j, i, Vec3f(d, d, d));
            normalRes.SetPixel(j, i, h.getNormal());

            // if(group->intersect(r, h, camera->getTMin()))
            // {
            //     if(h.getNormal().Dot3(r.getDirection()) >= 0)
            //     {
            //         if(shade_back)
            //         {
            //             Vec3f normal = h.getNormal();
            //             normal.Negate();
            //             h.set(h.getT(), h.getMaterial(), normal, r);
            //         }
            //         else
            //         {
            //             continue;
            //         }
            //     }

            //     // Vec3f color = rayTracer.traceRay(r, camera->getTMin(), 0, 0, 1, h);
            //     // colorRes.SetPixel(j, i, color);

            //     float d = (h.getT() - depth_min) / (depth_max - depth_min);
            //     d = max(0.0f, min(1.0f, 1.0f-d));
            //     depthRes.SetPixel(j, i, Vec3f(d, d, d));

            //     normalRes.SetPixel(j, i, h.getNormal());
            //     //cout << "intersect " << h << endl;
            // }
            
        }
    }
    if(output_file != NULL)
        colorRes.SaveTGA(output_file);
    if(depth_file != NULL)
        depthRes.SaveTGA(depth_file);
    if(normal_file != NULL)
        normalRes.SaveTGA(normal_file);
}

void traceRay(float x, float y)
{
    Camera* camera = sceneParser->getCamera();
	RayTracer raytracer(sceneParser);

	Vec2f point(y, x);
	Ray rayTemp = camera->generateRay(point);
	Hit hit_result;
	Vec3f Color = raytracer.traceRay(rayTemp, camera->getTMin(), 0, 1, 0, hit_result);
}

int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-input"))
        {
            i++;
            assert(i < argc);
            input_file = argv[i];
        }
        else if (!strcmp(argv[i], "-size"))
        {
            i++;
            assert(i < argc);
            width = atoi(argv[i]);
            i++;
            assert(i < argc);
            height = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-output"))
        {
            i++;
            assert(i < argc);
            output_file = argv[i];
        }
        else if (!strcmp(argv[i], "-depth"))
        {
            i++;
            assert(i < argc);
            depth_min = atof(argv[i]);
            i++;
            assert(i < argc);
            depth_max = atof(argv[i]);
            i++;
            assert(i < argc);
            depth_file = argv[i];
        }
        else if (!strcmp(argv[i], "-normals"))
        {
            i++;
            assert(i < argc);
            normal_file = argv[i];
        }
        else if (!strcmp(argv[i], "-shade_back"))
        {
            shade_back = true;
        }
        else if (!strcmp(argv[i], "-gui"))
        {
            is_gui = true;
        }
        else if (!strcmp(argv[i], "-gouraud"))
        {
            gouraud = true;
        }
        else if (!strcmp(argv[i], "-tessellation"))
        {
            i++;
            assert(i < argc);
            theta_steps = atof(argv[i]);
            i++;
            assert(i < argc);
            phi_steps = atof(argv[i]);
        }
        else if (!strcmp(argv[i], "-shadows"))
        {
            shadow = true;
        }
        else if (!strcmp(argv[i], "-bounces"))
        {
            i++;
            assert(i < argc);
            max_bounce = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-weight"))
        {
            i++;
            assert(i < argc);
            cutoff_weight = atof(argv[i]);
        }
        else
        {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }
    glutInit(&argc, argv);

    assert(depth_max > depth_min);
    sceneParser = new SceneParser(input_file);
    if(is_gui)
    {
        //cout << "aaa" << endl;
        canvas.initialize(sceneParser, render, traceRay);
    }
    else
    {
        render();
    }
    cout << "deleteParser" << endl;
    delete sceneParser;
    cout << "return 0" << endl;
    return 0;
}