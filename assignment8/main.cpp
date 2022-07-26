#include <stdio.h>
#include <assert.h>
using namespace std;

#include "arg_parser.hpp"
#include "glCanvas.hpp"
#include "spline_parser.hpp"

#include <OpenGL/gl.h>
//#include <GL/glew.h>
#include <OpenGL/glu.h>
//#include <GLFW/glfw3.h>
#include <GLUT/glut.h>

// ====================================================================
// ====================================================================

int main(int argc, char *argv[]) {

  // parse the command line arguments & input file
  ArgParser *args = new ArgParser(argc,argv);
  SplineParser* splines = new SplineParser(args->input_file);
  cout << "init" << endl;
  // launch curve editor!
  if (args->gui) {
    glutInit(&argc, argv);
    GLCanvas glcanvas;
    glcanvas.initialize(args,splines);
    // this never returns...
  }

  // output as required
  splines->SaveBezier(args);
  splines->SaveBSpline(args);
  splines->SaveTriangles(args);

  // cleanup
  delete args;
  delete splines;
  return 0;
}

// ====================================================================
// ====================================================================


