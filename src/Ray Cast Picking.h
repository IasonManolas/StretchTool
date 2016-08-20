#pragma once
#include <iostream>   // For string handling
#include <stdlib.h>	  // Just for some standard functions
#include <stdio.h>    // Just for some ASCII messages
#include <glut.h>     // An interface and windows management library
#include "windows.h"
#include "tiny_obj_loader.h"

#include "armadillo-6.200.5\include\armadillo.h"

#include "Ray.h"
#include "Triangle.h"
#include "Plane.h"
#include "Vector3D.h"
using namespace arma;
using namespace std;
using namespace tinyobj;
extern int pixelX,pixelY; //tells the compiler to search for a definition of those two variables i an other file and use the definition found
extern Ray clickedRay;
extern tinyobj::mesh_t& mesh;
void deviceCoords(int pixelWidth,int pixelHeight,double &xCoord,double &yCoord);
vec ray_world_direction(void);
void drawIntersectingPoint();
void drawIntersectingTriangle();
void doublePointRadius();
void halfPointRadius();