#include "Plane.h"


Plane::Plane(void)
{
}

Plane::Plane(Vector3D normal,Vector3D pointOnPlane)
{
	n=normal;
	a=normal.x;
	b=normal.y;
	c=normal.z;
	d=-(a*pointOnPlane.x+b*pointOnPlane.y+c*pointOnPlane.z);
}
Plane::Plane(Triangle tri)
{
	n=tri.n;
	a=n.x;
	b=n.y;
	c=n.z;
	Vector3D pointOnPlane=tri.v1;
	d=-(a*pointOnPlane.x+b*pointOnPlane.y+c*pointOnPlane.z);
}
Plane::Plane(tinyobj::mesh_t plane)
{
	float nx=plane.normals[0],ny=plane.normals[1],nz=plane.normals[2];
	n.Set(nx,ny,nz);
	a=n.x;
	b=n.y;
	c=n.z;
	Vector3D pointOnPlane;
	pointOnPlane.Set(plane.positions[0],plane.positions[1],plane.positions[2]);
	d=-(a*pointOnPlane.x+b*pointOnPlane.y+c*pointOnPlane.z);
}
Vector3D Plane::GetPointOnPlane()
{
	Vector3D v;
	v.Set(5,5,5);
	float dist=v*(*this).n;
	return v-dist*(*this).n;
}
bool Plane::isOnPositive(Vector3D p)
{
	return a*p.x+b*p.y+c*p.z+d>=0;
}
Plane::~Plane(void)
{
}

