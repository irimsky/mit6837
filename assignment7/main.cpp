#include <iostream>
using namespace std;

#include "scene_parser.hpp"
#include "../headers/image.hpp"
#include "../headers/camera.hpp"
#include "../headers/objects/group.hpp"
#include "../headers/light.hpp"
#include "glCanvas.hpp"
#include "raytracer.hpp"
#include "../headers/global.hpp"
#include "../headers/raytracing_stats.hpp"
#include "../headers/sampler.hpp"
#include "../headers/filter.hpp"


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
Grid* grid;
Sampler* sampler = NULL;
Filter* filter = NULL;
char *sample_file = NULL;
int sample_zoom = 20;
char *filter_file = NULL;
int filter_zoom = 1;


void render()
{
    Camera* camera = sceneParser->getCamera();
    Group* group = sceneParser->getGroup();
    Image colorRes(width, height), depthRes(width, height), normalRes(width, height);
    Film* film;
    if(num_samples > 1)
        film = new Film(width, height, num_samples);

    colorRes.SetAllPixels(sceneParser->getBackgroundColor());

    if(stats)
        RayTracingStats::Initialize(width, height, sceneParser->getGroup()->getBoundingBox(),
        nx, ny, nz);
        

    RayTracer rayTracer(sceneParser, grid);

    for(int i=0;i<width;++i)
    {
        for(int j=0;j<height;++j)
        {
            std::cout << i << ' ' << j << std::endl;
            Vec3f color;            
            Hit h;
            Vec2f center = Vec2f(i, j);
            if(sampler != NULL)
            {
                Vec3f sumColor = Vec3f(0, 0, 0);
                for(int k=0;k<num_samples;++k)
                {
                    Vec2f offset = sampler->getSamplePosition(k);
                    Vec2f point; 
                    Vec2f::Add(point, center, offset);
                    point.Divide(width, height);
                    float x = point.x(), y = point.y();
                    x = min(max(0.0f, x), 1.0f);
                    y = min(max(0.0f, y), 1.0f);
                    point.Set(x, y);
                    // cout << x << ' ' << y << endl;
                    Ray r = camera->generateRay(point);
                    Hit h;
                    Vec3f temp = rayTracer.traceRay(r, camera->getTMin(), 0, 1, 1, h);
                    if(film)
                        film->setSample(i, j, k, offset, temp);
                    else
                        sumColor += temp;
                    // cout << temp << ' ';
                    // 
                }
                
                // 
                if(film && filter)
                    colorRes.SetPixel(i, j, filter->getColor(i, j, film));
                else
                {
                    color = sumColor;
                    color /= (float)num_samples;
                    colorRes.SetPixel(i, j, color);
                }
            }
            else
            {
                Vec2f::Add(center, Vec2f(0.5f, 0.5f), center);
                center.Divide(width, height);
                Ray r = camera->generateRay(center);
                color = rayTracer.traceRay(r, camera->getTMin(), 0, 1, 1, h);
                colorRes.SetPixel(i, j, color);
            }

            
            
            float d = (h.getT() - depth_min) / (depth_max - depth_min);
            d = max(0.0f, min(1.0f, 1.0f-d));
            depthRes.SetPixel(i, j, Vec3f(d, d, d));
            normalRes.SetPixel(i, j, h.getNormal());

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
    if (sample_file != NULL)
        film->renderSamples(sample_file, sample_zoom);
    if (filter_file != NULL)
        film->renderFilter(filter_file, filter_zoom, filter);

    if(stats)
        RayTracingStats::PrintStatistics();
}

void traceRay(float x, float y)
{
    Camera* camera = sceneParser->getCamera();
	RayTracer raytracer(sceneParser, grid);

	Vec2f point(x, y);
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
        else if (!strcmp(argv[i], "-grid"))
        {
            i++;
            assert(i < argc);
            nx = atoi(argv[i]);
            i++;
            assert(i < argc);
            ny = atoi(argv[i]);
            i++;
            assert(i < argc);
            nz = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-visualize_grid"))
        {
            visualize_grid = true;
        }
        else if (!strcmp(argv[i], "-stats"))
        {
            stats = true;
        }
        else if (!strcmp(argv[i], "-random_samples"))
        {
            i++;
            assert(i < argc);
            num_samples = atoi(argv[i]);
            sampler = new RandomSampler();
        }
        else if (!strcmp(argv[i], "-uniform_samples"))
        {
            i++;
            assert(i < argc);
            num_samples = atoi(argv[i]);
            sampler = new UniformSampler(num_samples);
        }
        else if (!strcmp(argv[i], "-jittered_samples"))
        {
            i++;
            assert(i < argc);
            num_samples = atoi(argv[i]);
            sampler = new JitteredSampler(num_samples);
        }
        else if (!strcmp(argv[i], "-render_filter")) {

            i++; assert(i < argc);
            filter_file = argv[i];
            i++; assert(i < argc);
            filter_zoom = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-render_samples")) {
            i++; assert(i < argc);
            sample_file = argv[i];
            i++; assert(i < argc);
            sample_zoom = atoi(argv[i]);
        }
        else if (!strcmp(argv[i], "-box_filter")) {
            i++; assert(i < argc);
            float r = atof(argv[i]);
            filter = new BoxFilter(r);
        }
        else if (!strcmp(argv[i], "-tent_filter")) {
            i++; assert(i < argc);
            float r = atof(argv[i]);
            filter = new TentFilter(r);
        }
        else if (!strcmp(argv[i], "-gaussian_filter")) {
            i++; assert(i < argc);
            float r = atof(argv[i]);
            filter = new GaussianFilter(r);
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

    cout << 'x' << endl;
    if(nx > 0)  // use grid
    {
        grid = new Grid(sceneParser->getGroup()->getBoundingBox(), nx, ny, nz);
        sceneParser->getGroup()->insertIntoGrid(grid, NULL);
    }
    else
        grid = NULL;
    

    if(is_gui)
    {
        canvas.initialize(sceneParser, render, traceRay, grid, visualize_grid);
    }
    else
    {
        render();
    }
    cout << "deleteParser" << endl;
    delete sceneParser;
    if(nx > 0) delete grid;
    cout << "return 0" << endl;
    return 0;
}