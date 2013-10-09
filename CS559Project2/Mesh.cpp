#include <iostream>
#include <fstream>
#include <vector>
#include "Mesh.h"
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

int main() {
	Mesh::newMars(10, 1, "mars_low_rez.txt");
	int i;
	cin >> i;
	return 0;
}

Mesh *Mesh::newMars(float radius, float radScale, char *filename) {
	ifstream inFile(filename);
	if (inFile.is_open()) {
		int width, height;
		inFile >> width;
		inFile >> height;
		cout << "file is " << width << 'x' << height << endl;
		vector<vector<float>> radii(height);
		for (int c = 0; c < height; c++) {
			radii[c] = vector<float>(width);
			for (int d = 0; d < width; d++) {
				if (!(inFile >> radii[c][d])) {
					cout << "Failed to read "<< c << ',' << d << endl;
					return NULL;
				}
			}
		}
		inFile.close();

		cout << "done reading" << endl;

		return Mesh::newMars(radius, radScale, radii);
	} else {
		cerr << "Could not open file '" << filename << "'!" << endl;
		return NULL;
	}
}

Mesh *Mesh::newMars(float radius, float radScale,
					vector<vector<float>> radii) {

						

	//stacks are the rows of the mesh
	int stacks = radii.size();

	//Assumes that the 2D has the same amount of slices for each stack
	//Slices are the columns of the mesh
	int slices = radii[0].size();

	//vect2D points(stacks);
	vector<vec3> points(stacks*slices);

	for(int i = 0; i< stacks; i++)
	{
		//vect2D points(slices);
		//vector<vec3> rows(stacks);

		for( int j = 0; j < slices; j++)
		{
			float r = radius + radScale*radii[i][j];
			float theta = 360.0f * ((float) j / (float)slices);
			float phi = 360.0f * ((float) i / (float)stacks);

			float x = r*sin(theta)*sin(phi);
			float y = r*cos(theta)*sin(phi);
			float z = cos(phi);
			points.push_back(vec3(x,y,z));
		}
		//points.push_back(rows);
		//rows.clear();
	}

	return NULL;
}

Mesh::~Mesh() {
	delete points;
	delete trigs;
	delete normals;
}