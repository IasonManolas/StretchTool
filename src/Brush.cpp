#include "Brush.h"


Brush::Brush(void)
{
}

Brush::Brush(Vector3D colour, float power, float radius, bool permeable, int brushType)
{
	Brush::colour = colour;
	Brush::power = power;
	Brush::radius = radius;
	Brush::permeable = permeable;
	Brush::brushType = brushType;

}

void Brush::Set(Vector3D colour, float power, float radius, bool permeable,int brushType )
{
	Brush::colour=colour;
	Brush::power=power;
	Brush::radius=radius;
	Brush::permeable = permeable;
	Brush::brushType = brushType;
	
}
float Brush::Contains(Vector3D p)
{
	float distance_P_ray = clickedRay.distance(p);
	if (distance_P_ray < radius) return distance_P_ray;
	return -1;
}

void Brush::findVerticesUnderBrush()
{
	Vector3D white(1, 1, 1);
	Vector3D vertColour(colour.x, colour.y, colour.z);
	for (int i = 0; i < mesh.positions.size(); i+=3)
	{
		 
		Vector3D pos(mesh.positions[i], mesh.positions[i + 1], mesh.positions[i + 2]);
		float distFromCenter = Contains(pos); //center of cylinder
		if (distFromCenter == -1) continue; //whichever is outside cylinder continue to next vertex
		
		
			if (permeable)
			{
				if (brushType == 0) {
					float weight = vertexWeights[i / 3];
					if (weight<exp(-distFromCenter / radius))
						vertexWeights[i / 3] = exp(-distFromCenter / radius);
				}

				float Rcrt = radius / 3.0 + power*(radius - radius / 3.0);

				if (distFromCenter>Rcrt)
				{
					Vector3D fadingColour;
					float t = (distFromCenter - Rcrt) / (radius - Rcrt);
					fadingColour = colour + t*(white - colour);
					vertColour.Set(fadingColour.x, fadingColour.y, fadingColour.z);
				}

				if (sqrt(pow(perVerColour[i], 2) + pow(perVerColour[i + 1], 2) + pow(perVerColour[i + 2], 2)) > Magnitude(vertColour))
				{
					perVerColour[i] = vertColour.x;
					perVerColour[i + 1] = vertColour.y;
					perVerColour[i + 2] = vertColour.z;
				}
			}
			else
			{
				//angle check
				Vector3D vertNormal(mesh.normals[i], mesh.normals[i + 1], mesh.normals[i + 2]);

				if (pos.isVertexVisible(cameraPos, vertNormal))
				{
					if (brushType == 0) {
						float weight = vertexWeights[i / 3];
						if (weight<exp(-distFromCenter / radius))
							vertexWeights[i / 3] = exp(-distFromCenter / radius);
					}
					float Rcrt = radius / 3.0 + power*(radius - radius / 3.0);
					if (distFromCenter > Rcrt)
					{
						Vector3D fadingColour;
						float t = (distFromCenter - Rcrt) / (radius - Rcrt);
						fadingColour = colour + t*(white - colour);
						vertColour.Set(fadingColour.x, fadingColour.y, fadingColour.z);
					}

					if (sqrt(pow(perVerColour[i], 2) + pow(perVerColour[i + 1], 2) + pow(perVerColour[i + 2], 2)) > Magnitude(vertColour))
					{
						perVerColour[i] = vertColour.x;
						perVerColour[i + 1] = vertColour.y;
						perVerColour[i + 2] = vertColour.z;
					}

				}


			}
		
	}

}
void Brush::draw()
{
	//-------------Task4a---------------
	// Take the points, normals and faces of the obj
	vector<float> &positions = mesh.positions;
	vector<float> &normals = mesh.normals;
	vector<unsigned> &indices = mesh.indices;
	glColor3f(1, 1, 1);
	if (normals.size() != positions.size())
	{
		cout << "The number of normals is not the same with number of points" << endl;
		return;
	}

	float *point;
	float *normal;
	glPushMatrix();
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < indices.size(); i++)
	{
		// Set the normal
		normal = &normals[3 * indices[i]];
		glNormal3fv((GLfloat*)normal);
		// Set the point
		glPushMatrix();
		point = &positions[3 * indices[i]];

		glColor3f(perVerColour[3 * indices[i]], perVerColour[3 * indices[i] + 1], perVerColour[3 * indices[i] + 2]);
		glVertex3fv((GLfloat*)point);
		glPopMatrix();
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_POINTS);
	for (int i = 0; i < indices.size(); i++)
	{

		// Set the normal
		normal = &normals[3 * indices[i]];
		glNormal3fv((GLfloat*)normal);
		// Set the point
		point = &positions[3 * indices[i]];

		glColor3f(perVerColour[3 * indices[i]], perVerColour[3 * indices[i] + 1], perVerColour[3 * indices[i] + 2]);

		glVertex3fv((GLfloat*)point);
	}
	glEnd();
	glPopMatrix();


}
void Brush::drawWeights()
{
	//-------------Task4a---------------
	// Take the points, normals and faces of the obj
	vector<float> &positions = mesh.positions;
	vector<float> &normals = mesh.normals;
	vector<unsigned> &indices = mesh.indices;
	glColor3f(1, 1, 1);
	if (normals.size() != positions.size())
	{
		cout << "The number of normals is not the same with number of points" << endl;
		return;
	}

	float *point;
	float *normal;
	glPushMatrix();
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < indices.size(); i++)
	{
		// Set the normal
		normal = &normals[3 * indices[i]];
		glNormal3fv((GLfloat*)normal);
		// Set the point
		glPushMatrix();
		point = &positions[3 * indices[i]];

		glColor3f(vertexWeights[indices[i]], 0,0);
		if (vertexWeights[indices[i]] == 0) glColor3f(1, 1, 1);
		glVertex3fv((GLfloat*)point);
		glPopMatrix();
	}
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_POINTS);
	for (int i = 0; i < indices.size(); i++)
	{

		// Set the normal
		normal = &normals[3 * indices[i]];
		glNormal3fv((GLfloat*)normal);
		// Set the point
		point = &positions[3 * indices[i]];

		glColor3f(vertexWeights[indices[i]], 0, 0);
		if (vertexWeights[indices[i]] == 0) glColor3f(1, 1, 1);

		glVertex3fv((GLfloat*)point);
	}
	glEnd();
	glPopMatrix();


}
Brush::~Brush(void)
{
}

void brushColourAssignment()
{
	cout << "\nColour assignment started.." << endl;
	cout << "Use values in [0,1]" << endl;

	float red=-1;
	while (red > 1 || red < 0){
		cout << "Enter red factor:";
		cin >> red;
	}
	
	float green=-1;
	while (green > 1 || green < 0)
	{
		cout << "Enter green factor:";
		cin >> green;
	}

	float	blue=-1;
	while (blue > 1 || blue < 0)
	{
		cout << "Enter blue factor:";
		cin >> blue;
	}

	brushColor.Set(red, green, blue);
}
void brushPowerAssignment()
{
	float	hardness = -1;
	while (hardness < 0 || hardness > 100)
	{
		cout << "Enter hardness factor [0,100]:";
		cin >> hardness;
	}

	
	brushPower = hardness/100.0;
}
void brushRadiusAssignment()
{
	cout << "\nRadius assignment started.." << endl;
	float radius=-1;
	while (radius <= 0)
	{
		cout << "Enter Radius:";
		cin >> radius;
	}
	brushRadius = radius;
}
void brushPermeabilityAssignment()
{
	cout << "\nPermeability assignment started.." << endl;
	string Permeability;
	while (Permeability.compare("yes") != 0  && Permeability.compare("no") != 0)
	{
		cout << "Shall object be Permeable? [yes/no]:";
		cin >> Permeability;
	}
	if (Permeability.compare("yes") == 0) isBrushPermeable = true;
	else isBrushPermeable = false;
}
void brushInitialization()
{
	brushColourAssignment();
	brushPowerAssignment();
	brushRadiusAssignment();
	brushPermeabilityAssignment();
	
}
void clearColours()
{
	
	perVerColour.assign(mesh.positions.size() , 1);
}
void clearWeights()
{
	vertexWeights.assign(mesh.positions.size()/3, 0);
}

void saveColouredModel()
{
	ofstream outputFile;
	cout << "Saving file.." << endl;

	outputFile.open("..\\Models\\colouredModel.obj", std::ios_base::trunc);
	//outputFile << "\n";
	for (int i = 0; i<perVerColour.size(); i+=3)
	{
		outputFile << "vc ";
		outputFile << perVerColour[i];
		outputFile << " ";
		outputFile << perVerColour[i+1];
		outputFile << " ";
		outputFile << perVerColour[i+2];
		outputFile << " \n";
	}
	cout << "Finished." << endl;

}
void loadColouredModel()
{
	
	const char*  filename = "..\\Models\\colouredModel.obj";
	cout << "\nStarted Loading "<<filename<<  endl;
	ifstream in(filename, ios::in);
	if (!in)
	{
		cerr << "Cannot open " << filename << endl; exit(1);
	}
	string line;
	perVerColour.clear();
	while (getline(in, line))
	{
		if (line.substr(0, 3) == "vc ")
		{
			istringstream s(line.substr(2));
			Vector3D vc; 
			s >> vc.x;
			s >> vc.y;
			s >> vc.z;
			
			perVerColour.push_back(vc.x);
			perVerColour.push_back(vc.y);
			perVerColour.push_back(vc.z);
		}
		else
		{
			/* ignoring this line */
		}
	}
	cout << "Finished." << endl;

}