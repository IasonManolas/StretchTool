#pragma once
#include "Vector3D.h"
#include <vector>
#include "Ray.h"
#include "tiny_obj_loader.h"
#include <glut.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#define PI 3.1415927
using namespace std;
extern tinyobj::mesh_t& mesh;
extern Ray clickedRay;
extern vector<float> perVerColour;
extern Vector3D cameraPos;
extern vector<float> vertexWeights;


//these are used so user controls the brush attributes
extern Vector3D brushColor;
extern float brushPower, brushRadius;
extern bool isBrushPermeable;
class Brush
{
public:
	Vector3D colour;
	float power;
	float radius;
	bool permeable;
	std::vector<int> verticesUnderBrush;//indice in positions (x coordinate)
	int brushType;
	Brush(void);
	Brush(Vector3D colour, float power, float radius, bool permeable, int brushType);
	void Set(Vector3D colour, float power, float radius, bool permeable, int brushType);

	float Contains(Vector3D);
	void draw();
	void drawWeights();
	void findVerticesUnderBrush();
	~Brush(void);
private:
	
};

void brushInitialization();
void changeBrushColour();
void brushColourAssignment();
void brushPowerAssignment();
void brushRadiusAssignment();
void brushPermeabilityAssignment();
void clearColours();
void saveColouredModel();
void loadColouredModel();
void clearWeights();