#include "AnchorList.h"


AnchorList::AnchorList()
{
	std::vector<AnchorPoint> newAnchorList;
	anchorList = newAnchorList;
	radius = 0.5;
}
void AnchorList::add(AnchorPoint anchorPoint)
{
	anchorList.push_back(anchorPoint);
}
void AnchorList::clear()
{
	anchorList.clear();
}
void AnchorList::doubleRadius()
{
	radius = 2.0 * radius;
}
void AnchorList::halfRadius()
{
	radius = radius/2.0;
}
bool AnchorList::Contains(int intersectionIndex)
{
	for (int i = 0; i < anchorList.size(); i++)
	{
		if (anchorList[i].indexInPos == intersectionIndex) return true;
	}
	return false;
}
void AnchorList::draw()
{
	for (int i = 0; i < anchorList.size(); i++)
	{
		Vector3D pos(mesh.positions[anchorList[i].indexInPos], mesh.positions[anchorList[i].indexInPos + 1], mesh.positions[anchorList[i].indexInPos + 2]);
		glPushMatrix();
		glColor3f(0, 0, 1);
		glTranslatef(pos.x, pos.y, pos.z);
		glutSolidSphere(radius, 5, 5);
		glPopMatrix();
	}
	
}
AnchorList::~AnchorList()
{
}
