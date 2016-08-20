#pragma once
#include "Vector3D.h"
#include "Plane.h"
#include "Triangle.h"
#include "tiny_obj_loader.h"
class Ray
{
public:
	Vector3D direction; // direction of ray
	Vector3D origin; // ray starting point
	
	Ray(void);
	Ray(Vector3D n,Vector3D p);
	bool Intersects(tinyobj::mesh_t,std::vector<std::vector<float>>& Intersections); // finds all ray-triangle intersections and adds them in Intersections,
																						//in format: index of first vertex of intersected triangle in indices=Intersections[i-th Intersection][0]
																						//distance t at which the intersection occurs=Intersections[i-th Intersection][0].
	int ClosestIntersectionTriangle(tinyobj::mesh_t mesh,std::vector<std::vector<float>> allIntersectingTriangles); //returns index in indices of closest triangle.allIntersectingTriangles[minDistIntersection][0]=index returned													
	bool closestVisibleVertex(tinyobj::mesh_t mesh, Vector3D cameraPos, int& indexInPositions);
	Vector3D getPointAt(float distance);		
	float distance(Vector3D);
	bool Intersects(Plane,float&);
	~Ray(void);																				
};

