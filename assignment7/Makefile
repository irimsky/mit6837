# Simple Makefile for 6.837 raycasting assignment
# for g++ on linux platforms

# ===============================================================

SRCS 	 	= main.cpp \
		  ../headers/matrix.cpp \
		  ../headers/image.cpp \
		  scene_parser.cpp \
		  ../headers/light.cpp \
		  ../headers/material.cpp \
		  ../headers/global.cpp \
		  glCanvas.cpp \
		  ../headers/objects/sphere.cpp \
		  ../headers/objects/plane.cpp \
		  ../headers/objects/triangle.cpp \
		  ../headers/objects/transform.cpp \
		  ../headers/camera.cpp \
		  ../headers/objects/group.cpp \
		  ../headers/objects/grid.cpp \
		  ../headers/boundingbox.cpp \
		  ../headers/raytracing_stats.cpp \
		  ../headers/film.cpp \
		  ../headers/filter.cpp \
		  ../headers/perlin_noise.cpp \
		  raytracer.cpp \
		  rayTree.cpp 

# ===============================================================

LIBS        = -lm -L/usr/local/Cellar/glfw/3.3.7/lib -L/usr/local/Cellar/glew/2.2.0_1/lib
INCLUDE 	= -I/usr/local/Cellar/glfw/3.3.7/include \
			  -I/usr/local/Cellar/glew/2.2.0_1/include \

LDFLAGS= -framework GLUT -framework OpenGL -framework Cocoa 
CC		= g++
EXE	  	= raytracer
OBJS =  $(SRCS:.C=.o)


# ===============================================================
# targets

.PHONY: all depend clean

all: depend $(EXE)

depend:
	$(CC) $(LDFLAGS) $(INCLUDE) -E -M $(SRCS) > Makefile.depend

clean: 
	-rm -f *~ *bak *.o  core.* Makefile.depend $(EXE) 

# ===============================================================
# compilation rules

$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) $(INCLUDE) -o $@ $(OBJS) $(LIBS) -w

.cpp.o:
	$(CC) $(LDFLAGS) $(INCLUDE) $< -c -o $@

# ===============================================================

-include Makefile.depend