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
	return NULL;
}

Mesh::~Mesh() {
	delete points;
	delete trigs;
	delete normals;
}