#include "Ray Cast Picking.h"

float pointRadiusA1=0.5;



void deviceCoords(int pixelWidth,int pixelHeight,double &xCoord,double &yCoord)
{
	GLfloat  viewPort[4];
	glGetFloatv(GL_VIEWPORT,viewPort);
	double viewportWidth=viewPort[2],viewportHeight=viewPort[3];
	/*cout<<"width: "<<viewportWidth<<endl;
	cout<<"Height: "<<viewportHeight<<endl;*/
	xCoord=(2.0*pixelWidth)/viewportWidth-1.0;
	yCoord=1.0-(2.0*pixelHeight)/viewportHeight;
}
void findCenterOfMass(mesh_t mesh,Vector3D &CM)
{
	float xSum=0,ySum=0,zSum=0;
	for(int i=0;i<mesh.positions.size();i+=3)
	{
		xSum+=mesh.positions[i];
		ySum+=mesh.positions[i+1];
		zSum+=mesh.positions[i+2];
	}
	CM.Set(xSum/=3.0,ySum/=3.0,zSum/=3.0);
	
}
vec ray_world_direction(void)
{
	//step1
	double deviceCoordX,deviceCoordY;
	deviceCoords(pixelX,pixelY,deviceCoordX,deviceCoordY); //normalised device coords
	vec ray_nds(2);
	ray_nds<<deviceCoordX<<endr<<deviceCoordY<<endr;
	//cout<<ray_nds<<endl;

	//step2
	vec ray_clip(4);//4 elements
	ray_clip<<-1.0<<endr<<1.0<<endr;
	ray_clip=join_vert(ray_nds,ray_clip);
	//cout<<ray_clip<<endl;
	
	//step3
	vec ray_eye(4);
	mat proj(4,4);
	proj=zeros<mat>(4,4);
	double tempProj[16];
	glGetDoublev(GL_PROJECTION_MATRIX,tempProj);

	proj(0,0)=tempProj[0];
	proj(1,1)=tempProj[5];
	proj(2,2)=tempProj[10];
	proj(2,3)=tempProj[14];
	proj(3,2)=tempProj[11];
	ray_eye=solve(proj,ray_clip);
	ray_eye(2)=-1.0;
	ray_eye(3)=0.0;
	
	//step4
	double tempViewMatrix[16];
	
	glGetDoublev(GL_MODELVIEW_MATRIX,tempViewMatrix);
	mat viewMatrix;
	viewMatrix<<tempViewMatrix[0]<<tempViewMatrix[4]<<tempViewMatrix[8]<<tempViewMatrix[12]<<endr
	<<tempViewMatrix[1]<<tempViewMatrix[5]<<tempViewMatrix[9]<<tempViewMatrix[13]<<endr
	<<tempViewMatrix[2]<<tempViewMatrix[6]<<tempViewMatrix[10]<<tempViewMatrix[14]<<endr
	<<tempViewMatrix[3]<<tempViewMatrix[7]<<tempViewMatrix[11]<<tempViewMatrix[15]<<endr;

	vec  ray_wor = solve(viewMatrix , ray_eye);
	ray_wor.resize(3);
	ray_wor = normalise (ray_wor);
	return ray_wor;
}
void drawIntersectingPoint() //draws a sphere a the intersection
{
	vector<vector<float>> intersectionTriangles;
	if(clickedRay.Intersects(mesh,intersectionTriangles)) //draws only if there is an intersection
	{
	int indexOfClosestIntersection=clickedRay.ClosestIntersectionTriangle(mesh,intersectionTriangles); //returns index in intersectionTriangles not in mesh
	Triangle intersectionTri(mesh,intersectionTriangles[indexOfClosestIntersection][0]);
	float distanceOfIntersPoint=intersectionTriangles[indexOfClosestIntersection][1];
	Vector3D intersectionPoint=clickedRay.getPointAt(distanceOfIntersPoint);
	glPushMatrix();
	glColor3f(1,0,0);
	glTranslatef(intersectionPoint.x,intersectionPoint.y,intersectionPoint.z);
	glutSolidSphere(pointRadiusA1,5, 5);
	glPopMatrix();
	}
}
void drawIntersectingTriangle()
{

	vector<vector<float>> intersectionTriangles;

	if(clickedRay.Intersects(mesh,intersectionTriangles)) //draws only if there is an intersection
	{
	int indexOfClosestIntersection=clickedRay.ClosestIntersectionTriangle(mesh,intersectionTriangles); //returns index in intersectionTriangles not in mesh
	Triangle intersectionTri(mesh,intersectionTriangles[indexOfClosestIntersection][0]);
	float distanceOfIntersPoint=intersectionTriangles[indexOfClosestIntersection][1];
	Vector3D intersectionPoint=clickedRay.getPointAt(distanceOfIntersPoint);

	glBegin(GL_TRIANGLES);
	glPushMatrix();
	glColor3f(1,0,0);
	glVertex3f(intersectionTri.v1.x,intersectionTri.v1.y,intersectionTri.v1.z);
	//glColor3f(0, 1, 0);
	glVertex3f(intersectionTri.v2.x,intersectionTri.v2.y,intersectionTri.v2.z);
	//glColor3f(0, 0, 1);
	glVertex3f(intersectionTri.v3.x,intersectionTri.v3.y,intersectionTri.v3.z);
	glPopMatrix();
	glEnd();
	}
}
void doublePointRadius()
{
	pointRadiusA1 = 2.0 * pointRadiusA1;
}
void halfPointRadius() 
{
	pointRadiusA1 = pointRadiusA1/2.0;
}