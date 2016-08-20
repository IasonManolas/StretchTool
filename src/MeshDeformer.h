#pragma once
#include "AnchorPoint.h"
#include <vector>
#include "Ray.h"
#include "Vector3D.h"
#include <glut.h>     // An interface and windows management library
#include "Plane.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>
#include <iterator>
using namespace std;

extern Ray clickedRay;
extern tinyobj::mesh_t& mesh;
extern Vector3D cameraPos;
extern bool mouseClickDown;
extern vector<float> vertexWeights;
extern vector<vector<int>> oneRingNeig;
class MeshDeformer
{
public:
	std::vector<float> initialPositions;
	vector<float> stretchWeights;
	int numberOfDeformationAreas;
	set<int> redBoarder; //periexei ta index twn koryfwn toy kokkinoy sunoroy
	set<int> pointsReached,searchBoarder,terminationBoarder,terminationVertsReached;
	set<int> badVertsTerminationBoarder;
	MeshDeformer();
	MeshDeformer(AnchorPoint, vector<float> weights);
	void deformMesh(AnchorPoint);
	void saveMesh();
	void loadMesh();
	bool shallMeshBeDeformed(AnchorPoint anchorPoint);
	void findBoarderVertices();
	void inDepthMeshSearch(set<int>& pointsReached,set<int>& searchBoarder,set<int> terminationBoarder,set<int>& terminationVertsReached);
	void findBadVertsTerminationBoarder();
	set<int> findSetIntersection(set<int> a,set<int> b);
	void correctBadPoints();
	void reset();
	~MeshDeformer();
private:
	void drawPoint(Vector3D, Vector3D,float);
	Vector3D dragAnchor(AnchorPoint);
	int numberOfWhiteAreas;
};

