#include "AnchorPoint.h"


AnchorPoint::AnchorPoint()
{
	radius = 5;
}

AnchorPoint::AnchorPoint(int intersectionIndex)
{
	
	//Vector3D p(mesh.positions[intersectionIndex], mesh.positions[intersectionIndex + 1], mesh.positions[intersectionIndex + 2]);
	radius = 0.0625;
	//pos = p;
	indexInPos = intersectionIndex;
}
void AnchorPoint::Set(int intersectionIndex)
{
	//Vector3D p(mesh.positions[intersectionIndex], mesh.positions[intersectionIndex + 1], mesh.positions[intersectionIndex + 2]);

	//pos = p;
	indexInPos = intersectionIndex;
}
void AnchorPoint::draw(float red,float green,float blue)
{
	Vector3D pos(mesh.positions[indexInPos], mesh.positions[indexInPos + 1], mesh.positions[indexInPos + 2]);

	glPushMatrix();
	glColor3f(red, green, blue);
	glTranslatef(pos.x, pos.y,pos.z);
	glutSolidSphere(radius, 5, 5);
	glPopMatrix();
}
void AnchorPoint::doubleRadius()
{
	radius = 2.0 * radius;
}
void AnchorPoint::halfRadius()
{
	radius = radius / 2.0;
}
AnchorPoint::~AnchorPoint()
{
}
