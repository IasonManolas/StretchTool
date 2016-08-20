#include "Ray.h"
#include <vector>
#include <iostream>
Ray::Ray(void)
{
}

Ray::Ray(const Vector3D dir,const Vector3D beginningPointOfRay)
{
	direction=dir;
	origin=beginningPointOfRay;
}
bool Ray::Intersects(Plane plane,float &t)
{
	float denom = Dot(direction,plane.n);
	if (abs(denom) > 0.0001f) // your favorite epsilon
	{
		t = Dot((plane.GetPointOnPlane() - origin),(plane.n)) / denom;
		if (t >= 0) return true; // you might want to allow an epsilon here too
	}
	return false;
}
bool Ray::Intersects(tinyobj::mesh_t mesh,std::vector<std::vector<float>>& allIntersectingTriangles)
{
	
	for(int i=0;i<mesh.indices.size();i+=3) // for every triangle of mesh
	{
		Vector3D v1,v2,v3;
		v1.Set(mesh.positions[3*mesh.indices[i]],mesh.positions[3*mesh.indices[i]+1],mesh.positions[3*mesh.indices[i]+2]);
		v2.Set(mesh.positions[3*mesh.indices[i+1]],mesh.positions[3*mesh.indices[i+1]+1],mesh.positions[3*mesh.indices[i+1]+2]);
		v3.Set(mesh.positions[3*mesh.indices[i+2]],mesh.positions[3*mesh.indices[i+2]+1],mesh.positions[3*mesh.indices[i+2]+2]);
		Triangle tempTri;
		tempTri.Set(v1,v2,v3);
		float t;
		if(tempTri.triangle_intersection(v1,v2,v3,origin,direction,t)==true) //if there is an ray intersection with i-th triangle
		{
			std::vector<float> intersectingTriangle;
			intersectingTriangle.push_back(i);
			intersectingTriangle.push_back(t);
			allIntersectingTriangles.push_back(intersectingTriangle);
		}	
	}
	if(allIntersectingTriangles.size()==0) return false;
	return true;
}

int Ray::ClosestIntersectionTriangle(tinyobj::mesh_t mesh,std::vector<std::vector<float>> allIntersectingTriangles) 
{
	if(allIntersectingTriangles.size()==0)  //no intersection found.
	{
		std::cout<<"there are no intersections!!!"<<std::endl;
		return -1;
	}
	else if(allIntersectingTriangles.size()==1) //one intersection triangle.
	{
		int i=0;
		return i;
	}
	
	else//more than one intersections. Closest of all has to be found.
	{
	float tmin=allIntersectingTriangles[0][1];
	int indexOfTmin=0;
	for(int i=1;i<allIntersectingTriangles.size();i++)
	{
		if(allIntersectingTriangles[i][1]<tmin)
		{
			tmin=allIntersectingTriangles[i][1];
			indexOfTmin=i;
		}
	}
		return indexOfTmin;
	
	}
}
Vector3D Ray::getPointAt(const float distance)
{
	return this->origin+this->direction*distance;
}

float Ray::distance(Vector3D p)
{
	return Magnitude(Cross(direction, p - origin));

}
bool Ray::closestVisibleVertex(tinyobj::mesh_t mesh,Vector3D cameraPos, int& indexInPositions)
{
	std::vector<std::vector<float>> intersectingTriangles;
	int closestInterTriangle=-1;
	if (Intersects(mesh, intersectingTriangles)) closestInterTriangle = ClosestIntersectionTriangle(mesh, intersectingTriangles);
	if (closestInterTriangle == -1) return false;
	int i = intersectingTriangles[closestInterTriangle][0];
	Vector3D v1, v2, v3;
	v1.Set(mesh.positions[3 * mesh.indices[i]], mesh.positions[3 * mesh.indices[i] + 1], mesh.positions[3 * mesh.indices[i] + 2]);
	v2.Set(mesh.positions[3 * mesh.indices[i + 1]], mesh.positions[3 * mesh.indices[i + 1] + 1], mesh.positions[3 * mesh.indices[i + 1] + 2]);
	v3.Set(mesh.positions[3 * mesh.indices[i + 2]], mesh.positions[3 * mesh.indices[i + 2] + 1], mesh.positions[3 * mesh.indices[i + 2] + 2]);
	float d1, d2, d3;
	Vector3D p = getPointAt(intersectingTriangles[closestInterTriangle][1]);
	
	Vector3D temp=p-v1;
	float minDist = Magnitude(temp);
	indexInPositions = 3 * mesh.indices[i];
	temp = p - v2;
	if (Magnitude(temp) < minDist) {
		minDist = Magnitude(temp);
		indexInPositions = 3 * mesh.indices[i + 1];
	}
	temp = p - v3;
	if (Magnitude(temp) < minDist)
	{
		
		indexInPositions = 3 * mesh.indices[i + 2];
	}
	return true;

}
Ray::~Ray(void)
{
}
