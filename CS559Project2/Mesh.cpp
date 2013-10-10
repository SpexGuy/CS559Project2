#include <iostream>
#include <fstream>
#include <vector>
#include "Mesh.h"
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

int main() {
	Mesh::newMars(10, 1, "mars_low_rez.txt");
	cout << "worked!" << endl;
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
		assert(false);
		return NULL;
	}
}

Mesh *Mesh::newMars(float radius, float radScale,
					vector<vector<float>> radii)
{
	//Assumes that the 2D has the same amount of slices for each stack
	//Slices are the columns of the mesh

	int height = radii.size();
	assert(height > 0);
	int width = radii[0].size();
	assert(width > 0);

	vector<vec3> points(height*width + 2);

	for(int i = 0; i< height; i++) {
		for( int j = 0; j < width; j++) {
			float r = radius + radScale*radii[i][j];
			float theta = 360.0f * ((float) j / (float)width);
			float phi = 180.0f * (float(i+1) / float(height+1));

			float x = r*sin(theta)*sin(phi);
			float y = r*cos(theta)*sin(phi);
			float z = cos(phi);
			points.push_back(vec3(x,y,z));
		}
	}
	points.push_back(vec3(0, 0, radius));
	points.push_back(vec3(0, 0, -radius));

	vector<ivec3> trigs(height*width);
	for (int c = 0; c < height-1; c++) {
		for (int d = 0; d < width; d++) {
			int tlIndex = c*width + d;
			int blIndex = tlIndex + width;
			int trIndex = c*width + (d+1)%width;
			int brIndex = trIndex + width;
			trigs.push_back(ivec3(trIndex, tlIndex, blIndex));
			trigs.push_back(ivec3(blIndex, brIndex, trIndex));
		}
	}
	int botRow = (height-1)*width;
	for (int c = 0; c < width; c++) {
		trigs.push_back(ivec3(c, (c+1)%width, points.size()-2));
		trigs.push_back(ivec3(botRow + c, botRow + (c+1)%width, points.size()-1));
	}

	return new Mesh(points, trigs);
}

Mesh::Mesh(vector<vec3> points, vector<ivec3> trigs) {
	this->points = points;
	this->trigs = trigs;
	//TODO: compute normals
}

Mesh::~Mesh() {
	//delete points;
	//delete trigs;
	//delete normals;
}