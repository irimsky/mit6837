#include <iostream>
using namespace std;

#include "scene_parser.hpp"
#include "../headers/image.hpp"
#include "../headers/camera.hpp"
#include "../headers/objects/group.hpp"

char *input_file = NULL;
int width = 100;
int height = 100;
char *output_file = NULL;
float depth_min = 0;
float depth_max = 1;
char *depth_file = NULL;

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
        else
        {
            printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
            assert(0);
        }
    }

    assert(depth_max > depth_min);

    SceneParser sceneParser(input_file);
    Camera* camera = sceneParser.getCamera();
    Group* group = sceneParser.getGroup();
    Image colorRes(width, height), depthRes(width, height);
    
    for(int i=0;i<width;++i)
    {
        for(int j=0;j<height;++j)
        {
            //std::cout << i << ' ' << j << std::endl;
            Ray r = camera->generateRay(Vec2f((i + 0.5f)/width, (j + 0.5f)/height));
            Hit h;
            if(group->intersect(r, h, camera->getTMin()))
            {
                // std::cout << i << ' ' << j << std::endl;
                // cout << "intersect " << h << endl;
                // cout << h.getMaterial()->getDiffuseColor() << endl;
                colorRes.SetPixel(i, j, h.getMaterial()->getDiffuseColor());
                float d = (h.getT() - depth_min) / (depth_max - depth_min);
                d = max(0.0f, min(1.0f, 1.0f-d));
                depthRes.SetPixel(i, j, Vec3f(d, d, d));
                //cout << "intersect " << h << endl;
            }
            else {
                colorRes.SetPixel(i, j, sceneParser.getBackgroundColor());
                depthRes.SetPixel(i, j, Vec3f(0, 0, 0));
            }
            
        }
    }
    colorRes.SaveTGA(output_file);
    depthRes.SaveTGA(depth_file);

    return 0;
}