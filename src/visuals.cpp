//---8-------------------------------------------------------------------------//
//    Learning OpenGL: Εισαγωγικά Μαθήματα OpenGL                             //
//                                                                            //
//    Πλατφόρμα ανάπτυξης παραδειγμάτων                                       //
//                                                                            //
//    Εργαστήριο 4ο                                                           //
//----------------------------------------------------------------------------//

#include <iostream>   // For string handling
#include <stdlib.h>	  // Just for some standard functions
#include <stdio.h>    // Just for some ASCII messages
#include <glut.h>     // An interface and windows management library
#include "visuals.h"  // Header file for our OpenGL functions
#include "windows.h"
#include "tiny_obj_loader.h"
#include <algorithm>
#include "armadillo-6.200.5\include\armadillo.h"
#include "Ray Cast Picking.h"
#include "Ray.h"
#include "Triangle.h"
#include "Plane.h"
#include "Vector3D.h"
#include "Brush.h"
#include "AnchorPoint.h"
#include "AnchorList.h"
#include "MeshDeformer.h"
#include <list>
#define KEY_ESC 27
#define PI 3.1415927
using namespace arma;
using namespace std;
using namespace tinyobj;

/*******************************************************************************
  State Variables
 ******************************************************************************/

// Model variables
mesh_t tyra;
mesh_t tyra_norm;
mesh_t plane;
mesh_t teapot;
mesh_t bunny_low;
mesh_t icosahedron;
mesh_t test;


static float dx = 0.0;
static float dy = 0.0;
static float dz = 0.0;
static float ry = 0.0;
// Camera variables
static float rotFactor = 0.001;
static float zoomStep  = 10;
static float cdist = 250.0;
static float cx = 0.0;
static float cy = 0.0;
static float cz = cdist;
static float azimuthAngle  = 0.0;
static float altitudeAngle = PI/2.;
// Event handle variables
 bool mouseClickDown = false;
static int mx0;
static int my0;
bool cameraCanMove = false;
static int rotation=0;


//EXTERN VARIABLES
mesh_t& mesh = tyra;
int pixelX,pixelY;
Vector3D cameraPos;
Ray clickedRay;
vector<float> perVerColour;
vector<float> vertexWeights;
vector<vector<int>> oneRingNeig;
//Menu controls etc
int menuSelection=-1;
Vector3D brushColor(-1,-1,-1);
float brushPower=-1, brushRadius=-1;
bool isBrushPermeable;

//GLOBAL VARIABLES
Brush brush(brushColor, brushPower, brushRadius, isBrushPermeable, 1);
Vector3D red(1, 0, 0);
Brush weightBrush(red, brushPower, brushRadius, isBrushPermeable, 0);
AnchorPoint anchorPoint;
AnchorPoint dragPoint;
AnchorList anchorList;
MeshDeformer deformer;
vector<float> initialPos;
vector<float> initialNorm;

vector<float> tempWeights;
set<int> drawPoints1,drawPoints2;

/*******************************************************************************
  Implementation of Auxiliary Functions
 ******************************************************************************/

//------------------------------------------------------------------------------
void findOneRingNeighbours(mesh_t mesh)
{
	oneRingNeig.resize(mesh.positions.size()/3);
	for(int i=0;i<mesh.indices.size();i+=3)
	{
		oneRingNeig[mesh.indices[i]].push_back(mesh.indices[i+1]);
		oneRingNeig[mesh.indices[i]].push_back(mesh.indices[i+2]);

		oneRingNeig[mesh.indices[i+1]].push_back(mesh.indices[i]);
		oneRingNeig[mesh.indices[i+1]].push_back(mesh.indices[i+2]);

		oneRingNeig[mesh.indices[i+2]].push_back(mesh.indices[i]);
		oneRingNeig[mesh.indices[i+2]].push_back(mesh.indices[i+1]);
	}
	for(int i=0;i<oneRingNeig.size();i++)
	{
		sort( oneRingNeig[i].begin(), oneRingNeig[i].end() );
		oneRingNeig[i].erase( unique( oneRingNeig[i].begin(), oneRingNeig[i].end() ), oneRingNeig[i].end() );

	}
}
void calculateNormals(mesh_t &mesh)
{
    cout << endl << "Calculating Normals..." << endl;
    // Take the points, normals and faces of the obj
    vector<float> &positions = mesh.positions;
    vector<float> &normals = mesh.normals;
    vector<unsigned> &indices = mesh.indices;

    cout << "Before Calculations" << endl;
    cout << "Number of Positions: " << positions.size() << endl;
    cout << "Number of Normals: " << normals.size() << endl;
    cout << "Number of Indicies: " << indices.size() << endl << endl;
    
    // Set all normals (0.0 0.0 1.0)
    /*
    for (int i = 0; i < positions.size(); i+=3)
    {
        normals.push_back(0.0);
        normals.push_back(0.0);
        normals.push_back(1.0);
    }
    //*/

    //-------------Task4b---------------
	normals.resize(positions.size());
	for (int i = 0; i < indices.size(); i += 3)
	{
		// Get points of each triangle
		GLfloat *pt1 = (GLfloat*)&positions[3 * indices[i]];
		GLfloat *pt2 = (GLfloat*)&positions[3 * indices[i + 1]];
		GLfloat *pt3 = (GLfloat*)&positions[3 * indices[i + 2]];

		// Compute cross product pt12 x pt13
		GLfloat pt12[] = { *pt2 - *pt1, *(pt2 + 1) - *(pt1 + 1), *(pt2 + 2) - *(pt1 + 2) };
		GLfloat pt13[] = { *pt3 - *pt1, *(pt3 + 1) - *(pt1 + 1), *(pt3 + 2) - *(pt1 + 2) };

		GLfloat CP[] = { pt12[1] * pt13[2] - pt12[2] * pt13[1], pt12[2] * pt13[0] - pt12[0] * pt13[2], pt12[0] * pt13[1] - pt12[1] * pt13[0] };
		// Normalize the per triangle normal

		CP[0] = CP[0];
		CP[1] = CP[1];
		CP[2] = CP[2];
		// Add the calculated normal to the corresponding normal per vertex
		// Normal of the first point of current triangle (x, y, z)
		normals[3 * indices[i]] += CP[0];
		normals[3 * indices[i] + 1] += CP[1];
		normals[3 * indices[i] + 2] += CP[2];
		// Normal of the second point of current triangle (x, y, z)
		normals[3 * indices[i + 1]] += CP[0];
		normals[3 * indices[i + 1] + 1] += CP[1];
		normals[3 * indices[i + 1] + 2] += CP[2];
		// Normal of the third point of current triangle (x, y, z)
		normals[3 * indices[i + 2]] += CP[0];
		normals[3 * indices[i + 2] + 1] += CP[1];
		normals[3 * indices[i + 2] + 2] += CP[2];
	}

	for (int i = 0; i < positions.size(); i += 3)
	{
		float length = sqrt(pow(normals[i], 2) + pow(normals[i + 1], 2) + pow(normals[i + 2], 2));
		normals[i] = normals[i] / length;
		normals[i + 1] = normals[i + 1] / length;
		normals[i + 2] = normals[i + 2] / length;

	}
    cout << "After Calculations" << endl;
    cout << "Number of Positions: " << positions.size() << endl;
    cout << "Number of Normals: " << normals.size() << endl;
    cout << "Number of Indicies: " << indices.size() << endl;
}

//------------------------------------------------------------------------------

void readObj(string filename, mesh_t &mesh)
{
    //-------------Task4a---------------
    vector<shape_t> shapes;
    string err = LoadObj(shapes, filename.c_str());

    if (!err.empty())
    {
        // Print the error
        cerr << err << endl;
        exit(1);
    }
    else
    {
        // The .obj file might contain more than one shapes(meshes)
        // we are intrested only for the first one
        mesh = shapes[0].mesh;
        if (mesh.normals.empty())
        {
            cout << "Normals does not exist!" << endl;
            calculateNormals(mesh);
        }
    }
}

//------------------------------------------------------------------------------

void drawObj(mesh_t &mesh)
{
    //-------------Task4a---------------
    // Take the points, normals and faces of the obj
    vector<float> &positions = mesh.positions;
    vector<float> &normals = mesh.normals;
    vector<unsigned> &indices = mesh.indices;
	glColor3f(1,1,1);
    if (normals.size() != positions.size())
    {
        cout << "The number of normals is not the same with number of points" << endl;
        return;
    }

    float *point;
    float *normal;
	glBegin(GL_TRIANGLES);
	
    for (int i = 0; i < indices.size(); i++)
    {
        // Set the normal
        normal = &normals[3*indices[i]];
        glNormal3fv((GLfloat*) normal);
        // Set the point
		glPushMatrix();
        point = &positions[3*indices[i]];
		glVertex3fv((GLfloat*) point);
		glPopMatrix();
    }
    glEnd();
	
glBegin(GL_POINTS);
for (int i = 0; i < indices.size(); i++)
    {
        // Set the normal
        normal = &normals[3*indices[i]];
        glNormal3fv((GLfloat*) normal);
        // Set the point
        point = &positions[3*indices[i]];
		glVertex3fv((GLfloat*) point);
    }
    glEnd();

}

//------------------------------------------------------------------------------

string getExeDir()
{
    HMODULE hModule = GetModuleHandleW(NULL);
    WCHAR wPath[MAX_PATH];
    GetModuleFileNameW(hModule, wPath, MAX_PATH);
    char cPath[MAX_PATH];
    char DefChar = ' ';
    WideCharToMultiByte(CP_ACP, 0, wPath, -1, cPath, 260, &DefChar, NULL);
    string sPath(cPath);
    return sPath.substr(0, sPath.find_last_of("\\/")).append("\\");
}

/*******************************************************************************
  Implementation of Visual Functions
 ******************************************************************************/

void setup()
{ 
    // Parameter handling

    //-------------Task1a---------------
    // polygon rendering mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //-------------Task1b---------------
    // Make models with smoothed edges
    glShadeModel(GL_SMOOTH);
    //glShadeModel(GL_FLAT);
    // Blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Depth test
    glEnable(GL_DEPTH_TEST);
    // Renders a fragment if its z value is less or equal of the stored value
    glDepthFunc(GL_LEQUAL);
    glClearDepth(1);
  
    // Cull Faces
    // Don't draw triangles that cannot be seen
    //glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CW);
  
    // Automatically normalize normals of the model
    //glEnable(GL_NORMALIZE);
    //*
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set the camera
    gluLookAt(0, 40, 250, 0, 40, 0, 0, 1, 0);
    //*/

	anchorPoint.indexInPos=15;
    //-------------Task4a---------------
    //*
    // Read OBJ models
    // The mesh consists of positions, normals and indicies
    // we can access them using the following code:
    // tyra.positions[3*ith] returns the ith point
    // tyra.normals[3*ith] returns the ith normal
    // tyra.indices[ith] returns the index of the ith point
    // we have a new triangle every 3 indicies    
    readObj( getExeDir() + "..\\Models\\tyra.obj", tyra);
	//readObj(getExeDir() + "..\\Models\\plane.obj", plane);
	readObj(getExeDir() + "..\\Models\\teapot.obj", teapot);
	readObj(getExeDir() + "..\\Models\\bunny_low.obj", bunny_low);
    //*
    readObj(getExeDir() + "..\\Models\\icosahedron.obj", icosahedron);
    readObj(getExeDir() + "..\\Models\\plane.obj", plane);
	 readObj(getExeDir() + "..\\Models\\test.obj", test);
	//-------------Task4c---------------
    //*
    //readObj(getExeDir() + "..\\Models\\tyra_norm.obj", tyra);
    //*/
	 //Initialize brush
	initialPos=mesh.positions;
	initialNorm=mesh.normals;
	findOneRingNeighbours(mesh);

	 perVerColour.assign(mesh.positions.size(), 1);
	 vertexWeights.assign(mesh.positions.size() / 3, 0);
    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	
	 // Enable the Lighting
    glEnable(GL_LIGHTING);

    // Set up light source
    // Light 0
    GLfloat light0Ambient[]   = {0, 0, 0, 1.0};
    GLfloat light0Diffuse[]   = {1.0, 1.0, 1.0, 1.0};
    GLfloat light0Specular[]  = {1.0, 1.0, 1.0, 1.0};
    // If w=0 light is directional with (x,y,z) describe its direction
    GLfloat light0Position[]  = {50, 70, 100, 1};
    GLfloat light0Direction[] = {-1.0, -1.0, -1.0, 0.0};
  
    // Set Ambient Light
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light0Ambient);
    // Set Difuse Light
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0Diffuse);
    // Set Specular Light
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0Specular);
    // Set Position of the Light
    glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
    
    // Set Attenuation of the light a*x^2 + b*x + c
    //glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 2);
    //glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0001);
    //glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.00001);
    // Make Light a spot light setting the cutoff angle and direction
    //glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0Direction);
    //glLightf (GL_LIGHT0, GL_SPOT_EXPONENT, 16.0); // [0,128]

    // Enable Light 0
    glEnable(GL_LIGHT0);

	glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;
   glEnable ( GL_COLOR_MATERIAL ) ;
}

//------------------------------------------------------------------------------
void drawSets(set<int> setA,set<int> setB)
{
	drawPoints1=setA;
	drawPoints2=setB;
}
void anchorToDragBreadthFirst(set<int> roots,vector<int> &distance,Plane endPlane,Plane startingPlane,int &maxDist)
{

	int numberOfVerts=mesh.positions.size()/3;

	bool *visited=new bool[numberOfVerts];
	list<int> queue;

	for(int i=0;i<numberOfVerts;i++) 
	{
		if(roots.find(i)!=roots.end())
		{
			visited[i]=true;
			distance[i]=0;
			queue.push_back(i);
		}
		else
		{
			visited[i]=false;
			distance[i]=-1;
		}
	}

	queue.push_back(-1); //when -1 is reached currentRingDiameter will increase.
	
	int currentNode;	
	int currentRingDiameter=1;
	while(!(queue.size()==1))
	{
		currentNode=queue.front();
		queue.pop_front();
		if(currentNode==-1) 
		{
				currentRingDiameter++;
				queue.push_back(-1);
		}
		else
			for(int j=0;j<oneRingNeig[currentNode].size();j++)
			{
				int neighbourIndex=oneRingNeig[currentNode][j];
				Vector3D neigPos(mesh.positions[3*neighbourIndex],mesh.positions[3*neighbourIndex+1],mesh.positions[3*neighbourIndex+2]);
			
				if(!visited[neighbourIndex] && endPlane.isOnPositive(neigPos) && startingPlane.isOnPositive(neigPos)) 
				{
					visited[neighbourIndex]=true;
					queue.push_back(neighbourIndex);
					distance[neighbourIndex]=currentRingDiameter;
				}

			}
	}

	maxDist=currentRingDiameter;
}
bool isBoarderPoint(int pointIndex,Plane plane)
{
	for(int j=0;j<oneRingNeig[pointIndex].size();j++)
	{
		int neighbourIndex=oneRingNeig[pointIndex][j];
		Vector3D neighbourPos(mesh.positions[3*neighbourIndex],mesh.positions[3*neighbourIndex+1],mesh.positions[3*neighbourIndex+2]);
		if(!plane.isOnPositive(neighbourPos)) return true;
	}
	return false;
}
void pointsRelativeToPlanes(Plane p1,Plane p2,set<int> &between,set<int> &p1Side,set<int> &p2Side,set<int> &boarderPoints)
{
	for(int i=0;i<mesh.positions.size();i+=3)
	{
		Vector3D p(mesh.positions[i],mesh.positions[i+1],mesh.positions[i+2]);
		if(p1.isOnPositive(p) && p2.isOnPositive(p)) 
		{
				between.insert(i/3);
				if(isBoarderPoint(i/3,p2)) boarderPoints.insert(i/3);
		}
		else if(!p1.isOnPositive(p) && p2.isOnPositive(p)) p1Side.insert(i/3);
		else p2Side.insert(i/3);
	}

}
void normaliseDistancesVec(vector<int> v,vector<float> &normV,int max,set<int> dragSide)
{
	normV.resize(v.size());

	for(int i=0;i<v.size();i++)
	{
		if(dragSide.find(i)!=dragSide.end()) normV[i]=1;
		else if(v[i]>0) normV[i]=float(v[i])/float(max);
		else normV[i]=0;
	}
}
void stretchWeightsB3()
{
	Vector3D anchor(mesh.positions[anchorPoint.indexInPos],mesh.positions[anchorPoint.indexInPos+1],mesh.positions[anchorPoint.indexInPos+2]),
		drag(mesh.positions[dragPoint.indexInPos],mesh.positions[dragPoint.indexInPos+1],mesh.positions[dragPoint.indexInPos+2]);
	Vector3D anchorToDrag=drag-anchor,dragToAnchor=anchor-drag;
	Plane dragPlane(dragToAnchor.Normalize(),drag),anchorPlane(anchorToDrag.Normalize(),anchor);
	set<int> between,anchorSide,dragSide,anchorBetweenBoarderPoints;
	pointsRelativeToPlanes(dragPlane,anchorPlane,between,dragSide,anchorSide,anchorBetweenBoarderPoints);
	vector<int> distances;
	distances.resize(mesh.positions.size()/3);
	int maxOfDistances;
	anchorToDragBreadthFirst(anchorBetweenBoarderPoints,distances,dragPlane,anchorPlane,maxOfDistances);
	vector<float> normDistances;
	normaliseDistancesVec(distances,normDistances,maxOfDistances,dragSide);
	//drawSets(between,anchorBetweenBoarderPoints);
	tempWeights=normDistances;
}
void render()
{
    // Clean up the colour of the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
	//glShadeModel(GL_SMOOTH);
    glLoadIdentity();
    
    // Set the camera
    gluLookAt(cx, cy, cz,  // camera position
               0,    0,  0,  // target position
               0,     1,  0); // up vector
    
	cameraPos.Set(cx,cy,cz);

	//arxh armadillo
	vec ray_wor(3);
	ray_wor=ray_world_direction();
	//telos armadillo
	
	Vector3D rayDir;
	rayDir.Set(ray_wor(0),ray_wor(1),ray_wor(2));
	clickedRay.direction = rayDir;
	clickedRay.origin= cameraPos;
	
	drawObj(mesh);
	if (menuSelection == A1)
	{
		drawIntersectingPoint();//drawIntersectingTriangle();
		
	}
	else if (menuSelection == A2)
	{
		brush.Set(brushColor, brushPower, brushRadius, isBrushPermeable,1);
		brush.findVerticesUnderBrush();
		brush.draw();
	}
	else if (menuSelection == B1_ADD_WEIGHTS)
	{
		//glShadeModel(GL_FLAT);
		/*Vector3D red(1, 0, 0);
		Brush weightBrush(red, brushPower, brushRadius, isBrushPermeable,0);*/
		weightBrush.Set(red,brushPower,brushRadius,isBrushPermeable,0);
		weightBrush.findVerticesUnderBrush();
		weightBrush.drawWeights();
		

	}
	else if (menuSelection == B2 || menuSelection == B2_ADD_DRAG_POINT|| menuSelection == B2_ADD_ANCHOR)
	{
		if(menuSelection==B2_ADD_ANCHOR)
		{
			
			int intersectionIndex;
			if (clickedRay.closestVisibleVertex(mesh, cameraPos, intersectionIndex))
			{
				anchorPoint.Set(intersectionIndex);
				anchorPoint.draw(1,0,0);			
			}
		}
		else if(menuSelection==B2_ADD_DRAG_POINT)
		{
			int intersectionIndex;
			if (clickedRay.closestVisibleVertex(mesh, cameraPos, intersectionIndex))
			{
				dragPoint.Set(intersectionIndex);
				dragPoint.draw(0,1,0);			
			}
		}
		weightBrush.drawWeights();
	anchorPoint.draw(0,0,1);

	}
	else if (menuSelection == B3_DEFORM_MESH) //mia perioxh varwn
	{
weightBrush.drawWeights();
		deformer.deformMesh(anchorPoint);
		//dragPoint.draw(0,1,0);			
		anchorPoint.draw(0,0,1);
	}
	
	

	glutSwapBuffers();

}

//------------------------------------------------------------------------------

void resize(int w, int h)
{
    glMatrixMode(GL_PROJECTION); 
    // define the visible area of the window ( in pixels )
    if (h==0) h=1;
    glViewport(0, 0, w, h); 

    // Setup viewing volume
    glLoadIdentity();
    gluPerspective(60.0, (float)w/(float)h, 1.0, 500.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void breadthFirstWhiteRoot(int rootIndex,vector<vector<int>> oneRing,
	vector<float> weights,set<int>& whitesReached,set<int>& boarderPoints)
{
	int numberOfVerts=mesh.positions.size()/3;

	bool *visited=new bool[numberOfVerts];
	for(int i=0;i<numberOfVerts;i++) visited[i]=false;
	visited[rootIndex]=true;
	
	list<int> queue;
	queue.push_back(rootIndex);
	int currentNode=rootIndex;
	whitesReached.insert(rootIndex);
	while(!queue.empty())
	{
		currentNode=queue.front();
		queue.pop_front();
		for(int j=0;j<oneRing[currentNode].size();j++)
		{
			int neighbourIndex=oneRing[currentNode][j];
			if(!visited[neighbourIndex])
			{
				visited[neighbourIndex]=true;
				if(weights[neighbourIndex]!=0) 
				{
					boarderPoints.insert(neighbourIndex); 
					continue;
				}
				else 
				{
					queue.push_back(neighbourIndex);
					whitesReached.insert(neighbourIndex);

				}
				
			}
		}
	}

}
void breadthFirstRedRoot(int rootIndex,vector<vector<int>> oneRing,vector<float> weights,set<int>& allPointsReached,set<int>& boarderPoints)
{
	int numberOfVerts=mesh.positions.size()/3;

	bool *visited=new bool[numberOfVerts];
	for(int i=0;i<numberOfVerts;i++) visited[i]=false;

	
	list<int> queue;
	int currentNode=rootIndex;
	visited[currentNode]=true;
	allPointsReached.insert(currentNode);
	queue.push_back(currentNode);
	while(!queue.empty())
	{
		currentNode=queue.front();
		queue.pop_front();
		for(int j=0;j<oneRing[currentNode].size();j++)
		{
			int neighbourIndex=oneRing[currentNode][j];
			if(weights[neighbourIndex]==0) 
				{
					boarderPoints.insert(currentNode); 
				}
			if(!visited[neighbourIndex]&& weights[neighbourIndex]!=0 )
			{
				visited[neighbourIndex]=true;
				allPointsReached.insert(neighbourIndex);
				queue.push_back(neighbourIndex);
				
			}
		}
	}
}
void breadthRedAreaSearch(set<int> roots,vector<int> &distance,int &maxDist)
{

	int numberOfVerts=mesh.positions.size()/3;

	bool *visited=new bool[numberOfVerts];
	list<int> queue;

	for(int i=0;i<numberOfVerts;i++) 
	{
		if(roots.find(i)!=roots.end())
		{
			visited[i]=true;
			distance[i]=0;
			queue.push_back(i);
		}
		else
		{
			visited[i]=false;
			distance[i]=-1;
		}
	}

	queue.push_back(-1); //when -1 is reached currentRingDiameter will increase.
	
	int currentNode;	
	int currentRingDiameter=1;
	while(!(queue.size()==1))
	{
		currentNode=queue.front();
		queue.pop_front();
		if(currentNode==-1) 
		{
				currentRingDiameter++;
				queue.push_back(-1);
		}
		else
			for(int j=0;j<oneRingNeig[currentNode].size();j++)
			{
				int neighbourIndex=oneRingNeig[currentNode][j];
				Vector3D neigPos(mesh.positions[3*neighbourIndex],mesh.positions[3*neighbourIndex+1],mesh.positions[3*neighbourIndex+2]);
			
				if(!visited[neighbourIndex] && vertexWeights[neighbourIndex]!=0) 
				{
					visited[neighbourIndex]=true;
					queue.push_back(neighbourIndex);
					distance[neighbourIndex]=currentRingDiameter;
				}

			}
	}

	maxDist=currentRingDiameter;
}
void computeStretchWeightsB3()
{
	if(vertexWeights[anchorPoint.indexInPos/3]==0) //ap sto aspro
	{
		set<int> whiteReached,boarderPointsReached;
		breadthFirstWhiteRoot(anchorPoint.indexInPos/3,oneRingNeig,vertexWeights,whiteReached,boarderPointsReached);
		set<int> whites,reds;
		for(int i=0;i<vertexWeights.size();i++)
		{
			if(vertexWeights[i]==0) whites.insert(i);
			else reds.insert(i);
		}
		if(whites.size()==whiteReached.size())
		{
			cout<<"no deformation in this case"<<endl;
		}
		else
		{
			vector<int> distances;
			distances.resize(mesh.positions.size()/3);
			int maxDist;
			breadthRedAreaSearch(boarderPointsReached,distances,maxDist);
			for(int i=0;i<distances.size();i++)
			{
				
				if(distances[i]>0){ deformer.stretchWeights[i]=1.0-float(distances[i])/float(maxDist); vertexWeights[i]=deformer.stretchWeights[i];}

			}
			auto x=whiteReached.begin();
			for(int i=0;i<whiteReached.size();i++)
			{
				int currentPoint=*x;
				deformer.stretchWeights[currentPoint]=1;
				vertexWeights[currentPoint]=0;

				x++;
			}
			x=boarderPointsReached.begin();
			for(int i=0;i<boarderPointsReached.size();i++)
			{
				int currentPoint=*x;
				deformer.stretchWeights[currentPoint]=1;
				vertexWeights[currentPoint]=1;
				x++;

			}
			//vertexWeights=deformer.stretchWeights;
		}
	}

		else //ap se kokkino
		{
			set<int> ap;
			vector<int> distances;
			distances.resize(mesh.positions.size()/3);
			int maxDist;
			ap.insert(anchorPoint.indexInPos/3);
			breadthRedAreaSearch(ap,distances,maxDist);
			for(int i=0;i<distances.size();i++)
			{
				if(distances[i]>=0) deformer.stretchWeights[i]=1-float(distances[i])/float(maxDist);
				else deformer.stretchWeights[i]=0;
			}
		}
}

void getRedsBoardersAndWhites(set<int>& whites,set<int>& reds,set<int>& boarderReds)
{
	for(int i=0;i<vertexWeights.size();i++) 
	{
		if(vertexWeights[i]==0) whites.insert(i);
		else 
		{
			reds.insert(i);
			for(int j=0;j<oneRingNeig[i].size();j++)
			{
				if(vertexWeights[oneRingNeig[i][j]]==0) boarderReds.insert(i);
				
			}
			
		}
	}
}

void computeStretchWeightsB4()
{
	int Case,Subcase;
	set<int> posWhites,posReds;
	set<int> whites,reds,boarderReds;

	getRedsBoardersAndWhites(whites,reds,boarderReds);
	

	if(vertexWeights[anchorPoint.indexInPos/3]!=0)
	{
		set<int> redsReached,boarder,ap;
		breadthFirstRedRoot(anchorPoint.indexInPos/3,oneRingNeig,vertexWeights,
				redsReached,boarder);
		posReds=redsReached;
		// posWhites==empty
	}
	else // case==1 or case==2 or case==3
	{
		set<int> whiteArea1,redBoarder1;

		breadthFirstWhiteRoot(anchorPoint.indexInPos/3,oneRingNeig,vertexWeights,whiteArea1,redBoarder1);

		if(whiteArea1.size()==whites.size()) Case=1; 
		else //case==2 or case==3
		{
			/*set<int> tempBoarder,redArea1;
			int pointOnBoarder1=*redBoarder1.begin();
			breadthFirstRedRoot(pointOnBoarder1,oneRingNeig,vertexWeights,
				redArea1,tempBoarder);*/
			//drawSets(redBoarder1,redArea1);
			int pointOnWhiteArea2;
			auto x=whites.begin();
			for(int i=0;i<whites.size();i++)
			{
				int currentPoint=*x;
				if(whiteArea1.find(currentPoint)==whiteArea1.end()) 
				{
					pointOnWhiteArea2=*x;
					break;
				}
				x++;
			}
			set<int> whiteArea2,temp;

			breadthFirstWhiteRoot(pointOnWhiteArea2,oneRingNeig,vertexWeights,whiteArea2,temp); //to determine if case 2 or 3
			if(whiteArea1.size()+whiteArea2.size()==whites.size()) //CASE 2
			{
				Case=2;
				posWhites=whiteArea1;
				drawSets(whiteArea1,whiteArea2);
				int pointOnRedBoarder1=*redBoarder1.begin();
				set<int> redArea1,mixedBoarder;
				breadthFirstRedRoot(pointOnRedBoarder1,oneRingNeig,vertexWeights,redArea1,mixedBoarder);
				if(deformer.findSetIntersection(mixedBoarder,redBoarder1)==redBoarder1) cout<<"case2 subCase2"<<endl;
				else cout<<"case2 subCase1"<<endl;
			}
			else // CASE 3
			{
				int pointOnRedBoarder1=*redBoarder1.begin();
				set<int> redArea1,mixedBoarder;
				breadthFirstRedRoot(pointOnRedBoarder1,oneRingNeig,vertexWeights,redArea1,mixedBoarder);
			}
			//set<int> redBoarder2;
			//std::set_difference(tempBoarder.begin(), tempBoarder.end(), redBoarder1.begin(), redBoarder1.end(),
			//std::inserter(redBoarder2, redBoarder2.end()));


			//int pointOnWhiteArea2;
			//int x=*redBoarder2.begin();
			//int pointOnRedBoarder2=x;	
			//for(int i=0;i<oneRingNeig[pointOnRedBoarder2].size();i++)
			//{
			//	int neighbour=oneRingNeig[pointOnRedBoarder2][i];
			//	if(vertexWeights[neighbour]==0) pointOnWhiteArea2=oneRingNeig[pointOnRedBoarder2][i];
			//	break;
			//}
			//
			//set<int> whiteArea2,redBoarder3;
			//breadthFirstWhiteRoot(pointOnWhiteArea2,oneRingNeig,vertexWeights,
			//	whiteArea2,redBoarder3);
			
			
				
		}
	}
}
//------------------------------------------------------------------------------

void idle()
{
    //glMatrixMode(GL_MODELVIEW);
    // Make your changes here
    
    //-------------Task2c---------------
    // Light movement

    // After your changes rerender
    //glutPostRedisplay();
}

//------------------------------------------------------------------------------
void resetRay()
{
	pixelX=0;
	pixelY=0;
}
void menuSelect(int choice)
{
    //-------------Task2b---------------
    //*
	switch (choice)
	{
	case A1:
		menuSelection = choice;

		break;
	case A1_DOUBLE_POINT_RADIUS:
		menuSelection = A1;
		doublePointRadius();
		break;
	case A1_HALF_POINT_RADIUS:
		menuSelection = A1;
		halfPointRadius();
		break;
	case A2:
	{
		menuSelection = A2;
		Vector3D noColor(-1, -1, -1);
		brushInitialization();
		break;
	}
	case A2_COLOURS:
		menuSelection = A2;
		brushColourAssignment();
		break;
	case A2_HARDNESS:
		menuSelection = A2;
		brushPowerAssignment();
		break;
	case A2_RADIUS:
		menuSelection = A2;
		brushRadiusAssignment();
		break;
	case A2_PERMEABILITY:
		menuSelection = A2;
		brushPermeabilityAssignment();
		break;
	case A2_CLEAR:
		menuSelection = A2;
		clearColours();
		clearWeights();
		break;
	case A2_SAVE:
	{
		menuSelection = A2;
		saveColouredModel();
		Vector3D noColor(-1, -1, -1);
		if (brushPower == -1 && brushRadius == -1 && brushColor == noColor) brushInitialization();
		break;

	}
	case A2_LOAD:
	{
		menuSelection = A2;
		loadColouredModel();
		Vector3D noColor(-1, -1, -1);
		if (brushPower == -1 && brushRadius == -1 && brushColor == noColor) brushInitialization();
		break;

	}
	/*case B1_CHANGE_RADIUS:
		{
			brushRadiusAssignment();
		}
	case B1_CHANGE_PERM:
		isBrushPermeable=!isBrushPermeable;*/
	case B1_ADD_WEIGHTS:
	{
		resetRay();
		menuSelection = B1_ADD_WEIGHTS;
		brushRadiusAssignment();
		brushPermeabilityAssignment();		
		break;
	}
	
	case B2_ADD_ANCHOR:
	{
		resetRay();
		menuSelection = B2_ADD_ANCHOR;
		break;
	}
	case B2_ADD_DRAG_POINT:
	{
		
		resetRay();
		menuSelection = B2_ADD_DRAG_POINT;
		break;
	}
	case B2_DOUBLE_RADIUS:
	{
		menuSelection = B2;
		anchorPoint.doubleRadius();
		break;
	}
	case B2_HALF_RADIUS:
	{
		menuSelection = B2;
		anchorPoint.halfRadius();
		break;
	}

	case B3_DEFORM_MESH:
	{
		menuSelection = choice;
		deformer.stretchWeights.resize(mesh.positions.size()/3);
		clock_t tStart = clock();
		computeStretchWeightsB3();
		printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
		deformer.initialPositions=mesh.positions;
		break;
	}
	case B3_KEEP_CURRENT_MESH:
	{
		menuSelection = choice;
		deformer.initialPositions=mesh.positions;
		break;

	}
	case B4_DEFORM_MESH: // periptwsh dyo mh epikalyptwmenwn perioxwn
		{
			computeStretchWeightsB4();
			menuSelection=choice;
			break;
		}
	case RESET_MESH:
		menuSelection=choice;
		mesh.positions=initialPos;
		mesh.normals=initialNorm;
		deformer.reset();
		clearColours();
		clearWeights();
		break;
	case CAMERA_MOVEMENT:
		//menuSelection=choice;
		cameraCanMove ^= 1;
		break;
	case SAVE_DEFORMED_MODEL:
		deformer.saveMesh();
		//menuSelection=choice;
		break;
	case LOAD_DEFORMED_MODEL:
		deformer.loadMesh();
		calculateNormals(mesh);
		clearWeights();
		deformer.stretchWeights.clear();
		menuSelection=choice;
		break;
    case BOTH:
        break;
    case NONE:
        break;
    }

    glutPostRedisplay();
    //*/
}

//------------------------------------------------------------------------------

void keyboardDown(unsigned char key, int x, int y)
{
    switch(key)
    {
	case 'R':
    case 'r':
		rotation^=1;
        break;
    case 'W':
    case 'w':
        dz -= 1;
        break;
    case 'A':
    case 'a':
        dx -= 1;
        break;
    case 'S':
    case 's':
        dz += 1;
        break;
    case 'D':
    case 'd':
        dx += 1;
        break;
    case '+':
        cdist -= zoomStep;
        cx = cdist*sin(altitudeAngle)*sin(azimuthAngle);
        cy = cdist*cos(altitudeAngle);
        cz = cdist*sin(altitudeAngle)*cos(azimuthAngle);
        break;
    case '-':
        cdist += zoomStep;
        cx = cdist*sin(altitudeAngle)*sin(azimuthAngle);
        cy = cdist*cos(altitudeAngle);
        cz = cdist*sin(altitudeAngle)*cos(azimuthAngle);
        break;
    case KEY_ESC:
        exit(0);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

//------------------------------------------------------------------------------

void keyboardUp(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'W':
    case 'w':
		
        break;
    case 'A':
    case 'a':
        break;
    case 'S':
    case 's':
        break;
    case 'D':
    case 'd':
        break;
    case KEY_ESC:
        exit(0);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

//------------------------------------------------------------------------------

void keyboardSpecialDown(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        dy += 1;
        break;
    case GLUT_KEY_DOWN:
        dy -= 1;
        break;
    case GLUT_KEY_RIGHT:
        ry -= 1;
        break;
    case GLUT_KEY_LEFT:
        ry += 1;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

//------------------------------------------------------------------------------

void keyboardSpecialUp(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        break;
    case GLUT_KEY_DOWN:
        break;
    case GLUT_KEY_RIGHT:
        break;
    case GLUT_KEY_LEFT:
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

//------------------------------------------------------------------------------

void mouseClick(int button, int state, int x, int y)
{
    if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
    {
        mouseClickDown = true;
        mx0 = x;
        my0 = y;
		pixelX=x;
		pixelY=y;
    }
    else 
    {
        mouseClickDown = false;
    }
	glutPostRedisplay();
}

//------------------------------------------------------------------------------

void mouseMotion(int x, int y)
{
    if (mouseClickDown)
    {
		pixelX=x;
		pixelY=y;

		if (cameraCanMove)
		{

			// Calculate angles
			azimuthAngle -= (x - mx0)*rotFactor;
			altitudeAngle -= (y - my0)*rotFactor;
			// Set new camrea position
			cx = cdist*sin(altitudeAngle)*sin(azimuthAngle);
			cy = cdist*cos(altitudeAngle);
			cz = cdist*sin(altitudeAngle)*cos(azimuthAngle);
		}
		// Keep mouse x,y for next call
        mx0 = x;
        my0 = y;

        glutPostRedisplay();
    }
    //*/
}

