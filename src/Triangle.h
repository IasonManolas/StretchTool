#pragma once
#include "Vector3D.h"
#include "tiny_obj_loader.h"
class Triangle
{
public:
	Vector3D v1,v2,v3; //triangles vertices
	Vector3D n; //triangle normal
	Triangle(void);
	Triangle(Vector3D v1,Vector3D v2,Vector3D v3);
	Triangle(tinyobj::mesh_t mesh,int indexInIndices); // positions[3*indices[indexInIndices]] gives x-coord of first vertex of triangle// tinyObjLoader needed.
	Triangle& Set(Vector3D v1,Vector3D v2,Vector3D v3);
	bool triangle_intersection( const Vector3D V1,const Vector3D V2, const Vector3D V3,
	const Vector3D rayOrigin,const Vector3D rayDirection,float &t);      
                          

                                  
	~Triangle(void);
};

