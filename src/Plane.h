#pragma once
#include "Vector3D.h"
#include "Triangle.h"
#include "tiny_obj_loader.h"
class Plane
{
public:
	Vector3D n; // plane normal
	float a,b,c; // plane coefs
	float	d;  // planes offset
	
	Plane(void);
	Plane(Vector3D normal,Vector3D pointOnPlane);
	Plane(tinyobj::mesh_t plane);
	Plane(Vector3D ,Vector3D,Vector3D);
	Plane(Triangle);
	Vector3D GetPointOnPlane();
	bool isOnPositive(Vector3D);

	~Plane(void);
};

