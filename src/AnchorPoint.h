#pragma once
#include "tiny_obj_loader.h"
#include "Vector3D.h"
#include "Ray.h"
#include <glut.h> 

extern Vector3D cameraPos;
extern tinyobj::mesh_t& mesh;
class AnchorPoint
{
public:
	//Vector3D pos;
	float radius;
	int indexInPos;
	AnchorPoint(int);
	void Set(int intersectionIndex);
	AnchorPoint();
	void doubleRadius();
	void halfRadius();
	void draw(float,float,float);
	~AnchorPoint();
};

