#include "MeshDeformer.h"
MeshDeformer::MeshDeformer()
{
	
}
void MeshDeformer::findBoarderVertices()
{

	for(int i=0;i<vertexWeights.size();i++)
	{
		if(vertexWeights[i]!=0)
			for(int j=0;j<oneRingNeig[i].size();j++)
			{
				if(vertexWeights[oneRingNeig[i][j]]==0) 
					{
						redBoarder.insert(i); //vertex i colour of which is not white has a one ring neighbour that is white

					}
			}
	}
}
void MeshDeformer::inDepthMeshSearch(set<int>& pointsReached,set<int>& searchBoarder,set<int> terminationBoarder,set<int>& terminationVertsReached)
{
	auto x=searchBoarder.begin();
	int currentPoint=*x;
	pointsReached.insert(currentPoint);
	searchBoarder.erase(currentPoint);
	
	for(int i=0;i<oneRingNeig[currentPoint].size();i++)
	{
		int neig=oneRingNeig[currentPoint][i];
		if(pointsReached.find(neig)!=pointsReached.end()) continue;
		if(terminationBoarder.find(neig)==terminationBoarder.end())
			{
				searchBoarder.insert(neig);
				stretchWeights[neig]=1.0;
				inDepthMeshSearch(pointsReached,searchBoarder,terminationBoarder,terminationVertsReached);
				
		}
		else
		{
			pointsReached.insert(neig);
			terminationVertsReached.insert(neig);
		}
	}
	
	/*for(int i=0;i<boarderVertices.size();i++)
	{
	Vector3D pos(mesh.positions[3*boarderVertices[i]], mesh.positions[3*boarderVertices[i] + 1], mesh.positions[3*boarderVertices[i] + 2]);

	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslatef(pos.x, pos.y,pos.z);
	glutSolidSphere(1, 5, 5);
	glPopMatrix();
	}
*/
}
void MeshDeformer::findBadVertsTerminationBoarder() //these that stay behind when i drag
{
	for(int i=0;i<vertexWeights.size();i++)
	{
		if(vertexWeights[i]>0.99) badVertsTerminationBoarder.insert(i);
	}
}
set<int> MeshDeformer::findSetIntersection(set<int> a,set<int> b)
{
	set<int> intersection;
	set_intersection(a.begin(),a.end(),b.begin(),b.end(),
                  std::inserter(intersection,intersection.begin()));
	return intersection;
}
void MeshDeformer::correctBadPoints()
{

	auto x=pointsReached.begin(); 
	int currentPoint;
	for(int i=0;i<pointsReached.size();i++)
	{
		currentPoint=*x;
		stretchWeights[currentPoint]=1.0;
		x++;
	}
	
}
void MeshDeformer::deformMesh(AnchorPoint anchorPoint)
{
	mesh.positions=initialPositions;
	Vector3D dragedPos=dragAnchor(anchorPoint);
	Vector3D anchorPos(initialPositions[anchorPoint.indexInPos],initialPositions[anchorPoint.indexInPos+1],initialPositions[anchorPoint.indexInPos+2]);
	//Vector3D red(1,0,0);
	//drawPoint(dragedPos,red);

	Vector3D deformationVector;
	deformationVector=dragedPos-anchorPos;

	for(int i=0;i<stretchWeights.size();i++)
	{
		float weight=stretchWeights[i];
		Vector3D pos(mesh.positions[3*i],mesh.positions[3*i+1],mesh.positions[3*i+2]);
		Vector3D deformedPos=pos+weight*deformationVector;

		//if(3*i==anchorPoint.indexInPos) deformedPos=dragedPos;
		
		mesh.positions[3*i]=deformedPos.x;
		mesh.positions[3*i+1]=deformedPos.y;
		mesh.positions[3*i+2]=deformedPos.z;
	}
}
Vector3D MeshDeformer::dragAnchor(AnchorPoint anchorPoint)
{
	Vector3D anchorPos(initialPositions[anchorPoint.indexInPos],initialPositions[anchorPoint.indexInPos+1],initialPositions[anchorPoint.indexInPos+2]);
	Vector3D pToCam_Normal = (cameraPos - anchorPos).Normalize();
	Plane plane(pToCam_Normal, anchorPos);
	float t;
	clickedRay.Intersects(plane, t);

	Vector3D dragedPos = clickedRay.getPointAt(t);
	return dragedPos;
}


void MeshDeformer::drawPoint(Vector3D p,Vector3D colour,float anchorPointRadius)
{
	glPushMatrix();
	glColor3f(colour.x, colour.y, colour.z);
	glTranslatef(p.x, p.y, p.z);
	glutSolidSphere(anchorPointRadius, 5, 5);
	glPopMatrix();
}
void MeshDeformer::reset()
{
	stretchWeights.clear();
	redBoarder.clear(); //periexei ta index twn koryfwn toy kokkinoy sunoroy
	pointsReached.clear();
	searchBoarder.clear();
	terminationBoarder.clear();
	terminationVertsReached.clear();
	badVertsTerminationBoarder.clear();
}
MeshDeformer::~MeshDeformer()
{
}

void MeshDeformer::saveMesh()
{
	ofstream outputFile;
	cout << "Saving file.." << endl;

	outputFile.open("..\\Models\\deformedModel.obj", std::ios_base::trunc);
	//outputFile << "\n";
	for (int i = 0; i<mesh.positions.size(); i+=3)
	{
		outputFile << "v ";
		outputFile << mesh.positions[i];
		outputFile << " ";
		outputFile << mesh.positions[i+1];
		outputFile << " ";
		outputFile << mesh.positions[i+2];
		outputFile << " \n";
	}

	cout << "Finished." << endl;
}
void MeshDeformer::loadMesh()
{
	const char*  filename = "..\\Models\\deformedModel.obj";
	cout << "\nStarted Loading "<<filename<<  endl;
	ifstream in(filename, ios::in);
	if (!in)
	{
		cerr << "Cannot open " << filename << endl; exit(1);
	}
	string line;
	mesh.positions.clear();
	mesh.normals.clear();
	while (getline(in, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			istringstream s(line.substr(2));
			Vector3D v; 
			s >> v.x;
			s >> v.y;
			s >> v.z;
			
			mesh.positions.push_back(v.x);
			mesh.positions.push_back(v.y);
			mesh.positions.push_back(v.z);
		}
		
	}
	
	cout << "Finished." << endl;
}