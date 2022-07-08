// ========================================================
// ========================================================
// Some sample code you might like to use for parsing 
// command line arguments and the input IFS files

// sample command line:
// ifs -input fern.txt -points 10000 -iters 10 -size 100 -output fern.tga
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/matrix.hpp"
#include "../headers/image.hpp"


char *input_file = NULL;
int num_points = 10000;
int num_iters = 10;
int image_size = 100;
char *output_file = NULL;

int main(int argc, char* argv[])
{

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i],"-input")) {
            i++; assert (i < argc); 
            input_file = argv[i];
        } else if (!strcmp(argv[i],"-points")) {
            i++; assert (i < argc); 
            num_points = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-iters")) {
            i++; assert (i < argc); 
            num_iters = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-size")) {
            i++; assert (i < argc); 
            image_size = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-output")) {
            i++; assert (i < argc); 
            output_file = argv[i];
        } else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }

    // ========================================================
    // ========================================================
    // Some sample code you might like to use for
    // parsing the IFS transformation files

    // open the file
    FILE *input = fopen(input_file,"r");
    assert(input != NULL);

    // read the number of transforms
    int num_transforms; 
    fscanf(input,"%d",&num_transforms);

    // < DO SOMETHING WITH num_transforms >
    Matrix* mats = new Matrix[num_transforms];
    float* probability = new float[num_transforms];

    // read in the transforms
    for (int i = 0; i < num_transforms; i++) 
    {
        // float probability; 
        fscanf (input,"%f",&probability[i]);
        // Matrix m; 
        mats[i].Read3x3(input);
        // < DO SOMETHING WITH probability and m >
    }

    srand(time(NULL));
    Image* res = new Image(image_size, image_size);
    res->SetAllPixels(Vec3f(1, 1, 1));
    for(int i = 0;i < num_points;++i)
    {
        float x = drand48(), y = drand48();
        Vec4f vec(x, y, 0, 1.0);
        for(int it = 0; it < num_iters; ++it)
        {
            double x = drand48();
            double sum = 0.0;
            int j;
            for(j=0;j<num_transforms;++j)
            {
                sum += probability[j];
                if(sum > x)
                    break;
            }
            // std::cout << vec << std::endl;
            // std::cout << j << std::endl;
            // std::cout << mats[j] << std::endl;
            mats[j].Transform(vec);
            // std::cout << vec << std::endl;
            vec /= vec.w();
        }
        if(vec.x() >= 0 && vec.x() < 1 && vec.y() >= 0 && vec.y() < 1)
            res->SetPixel(vec.x() * image_size, vec.y() * image_size, Vec3f(0, 0, 0));
    }
    res->SaveTGA(output_file);
    // close the file
    fclose(input);

    return 0;
// ========================================================
// ========================================================
}

// ./ifs -input ./in/fern.txt -points 50000 -iters 30 -size 400 -output ./out/fern.tga
// ./ifs -input ./in/dragon.txt -points 50000 -iters 30 -size 400 -output ./out/dragon.tga
// ./ifs -input ./in/sierpinski_triangle.txt -points 50000 -iters 30 -size 200 -output ./out/sierpinski_triangle.tga
// ./ifs -input ./in/x.txt -points 50000 -iters 30 -size 400 -output ./out/x.tga

// ./ifs -input ./in/sierpinski_triangle.txt -points 50000 -iters 0 -size 400 -output ./out/sierpinski_triangle_0.tga
// ./ifs -input ./in/sierpinski_triangle.txt -points 50000 -iters 1 -size 400 -output ./out/sierpinski_triangle_1.tga
// ./ifs -input ./in/sierpinski_triangle.txt -points 50000 -iters 2 -size 400 -output ./out/sierpinski_triangle_2.tga
// ./ifs -input ./in/sierpinski_triangle.txt -points 50000 -iters 3 -size 400 -output ./out/sierpinski_triangle_3.tga
// ./ifs -input ./in/sierpinski_triangle.txt -points 50000 -iters 4 -size 400 -output ./out/sierpinski_triangle_4.tga




