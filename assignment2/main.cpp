#include <iostream>
using namespace std;

#include "scene_parser.hpp"
#include "../headers/image.hpp"
#include "../headers/camera.hpp"
#include "../headers/objects/group.hpp"
#include "../headers/light.hpp"

char *input_file = NULL;
int width = 100;
int height = 100;
char *output_file = NULL;
float depth_min = 0;
float depth_max = 1;
char *depth_file = NULL;
char *normal_file = NULL;
bool shade_back = false;

Vec3f calDirectionalLight(const Hit& hit, Light* light)
{
    Vec3f l, lightCol;
    float distance;
    light->getIllumination(hit.getIntersectionPoint(), l, lightCol, distance);
    // Vec3f v = camera.getCenter() - hit.getIntersectionPoint();
    Vec3f diffuse = hit.getMaterial()->getDiffuseColor();

    return lightCol * diffuse * max(hit.getNormal().Dot3(l), 0.0f);
}

Vec3f calAmbientLight(const Hit& hit, const Vec3f& ambient)
{
    Vec3f diffuse = hit.getMaterial()->getDiffuseColor();
    return diffuse * ambient;
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
    Image colorRes(width, height), depthRes(width, height), normalRes(width, height);

    
    for(int i=0;i<width;++i)
    {
        for(int j=0;j<height;++j)
        {
            std::cout << i << ' ' << j << std::endl;
            Ray r = camera->generateRay(Vec2f((i + 0.5f)/width, (j + 0.5f)/height));
            cout << r << endl;
            Hit h;
            if(group->intersect(r, h, camera->getTMin()))
            {
                if(h.getNormal().Dot3(r.getDirection()) >= 0)
                {
                    if(shade_back)
                    {
                        Vec3f normal = h.getNormal();
                        normal.Negate();
                        h.set(h.getT(), h.getMaterial(), normal, r);
                    }
                    else
                    {
                        colorRes.SetPixel(j, i, sceneParser.getBackgroundColor());
                        continue;
                    }
                }
                
                Vec3f sumColor(0, 0, 0);
                sumColor += calAmbientLight(h, sceneParser.getAmbientLight());
                for(int i=0;i<sceneParser.getNumLights(); ++i)
                {
                    Light* light = sceneParser.getLight(i);
                    sumColor += calDirectionalLight(h, light);
                }
                colorRes.SetPixel(j, i, sumColor);

                float d = (h.getT() - depth_min) / (depth_max - depth_min);
                d = max(0.0f, min(1.0f, 1.0f-d));
                depthRes.SetPixel(j, i, Vec3f(d, d, d));

                normalRes.SetPixel(j, i, h.getNormal());
                //cout << "intersect " << h << endl;
            }
            else {
                colorRes.SetPixel(j, i, sceneParser.getBackgroundColor());
            }
            
        }
    }
    if(output_file != NULL)
        colorRes.SaveTGA(output_file);
    if(depth_file != NULL)
        depthRes.SaveTGA(depth_file);
    if(normal_file != NULL)
        normalRes.SaveTGA(normal_file);


    return 0;
}