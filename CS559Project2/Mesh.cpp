#include <iostream>
#include <fstream>
#include <vector>
#include "Mesh.h"
#include "shader.h"
#include "object.h"
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

#define DEBUG

int main() {
	Mesh::newMars(10, 1, "mars_low_rez.txt");
	cout << "worked!" << endl;
	int i;
	cin >> i;
	return 0;
}

Mesh *Mesh::newMars(float radius, float radScale, char *filename){
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
#ifdef DEBUG
			if(i < 1 && j < 10)
				cout << "( " << x << ", " << y << "," << z << " )" <<endl;
#endif
			points.push_back(vec3(x,y,z));
		}
	}
#ifdef DEBUG
			
			cout << "Triangles" <<endl;
#endif
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

#ifdef DEBUG
			if(c < 1 && (d > width - 1 || d < 1))
			cout << "( " << trIndex << ", " << tlIndex << "," << blIndex << " )" <<endl;
#endif

			trigs.push_back(ivec3(blIndex, brIndex, trIndex));

#ifdef DEBUG
			if(c < 1 && (d > width - 1 || d < 1))
			cout << "( " << blIndex << ", " << brIndex << "," << trIndex << " )" <<endl;
#endif
		}
	}
	int botRow = (height-1)*width;
	for (int c = 0; c < width; c++) {
		trigs.push_back(ivec3(c, (c+1)%width, points.size()-2));
		trigs.push_back(ivec3(botRow + c, botRow + (c+1)%width, points.size()-1));
	}

	return new Mesh(points, trigs);
}

void Mesh::draw(const mat4 & projection,mat4 modelview, const ivec2 & size, const float time)
{
	//If there is an error return
	if (this->GLReturnedError("Mesh::Draw - on entry"))
		return;

	glEnable(GL_DEPTH_TEST);
	//for test 1 will will use the same rotate with time as top
	modelview = rotate(modelview, time * 30.0f, vec3(1.0f, 0.0f, 0.0f));
	modelview = rotate(modelview, time * 120.0f, vec3(0.0f, 1.0f, 0.0f));

	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));

	this->shaders[this->shader_index]->Use();
	this->GLReturnedError("Top::Draw - after use");
	this->shaders[this->shader_index]->CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm));
	this->GLReturnedError("Top::Draw - after common setup");
	glBindVertexArray(this->vertex_array_handle);
	glDrawElements(GL_TRIANGLES , this->vertex_indices.size(), GL_UNSIGNED_INT , &this->vertex_indices[0]);
	glBindVertexArray(0);

	this->GLReturnedError("Mesh::draw - after draw");
	glUseProgram(0);
	if (this->GLReturnedError("Mesh::draw - on exit"))
		return;
}
Mesh::Mesh(vector<vec3> points, vector<ivec3> trigs) {
	this->points = points;
	this->trigs = trigs;
	//TODO: compute normals
}

bool Mesh::initialize() {
	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->points.size() * sizeof(vec3), &this->points[0]))
	return false;

	/*	The VertexAttributesPCN class stores vertex attributes: position, color and normal in that order.

		Vertex attributes are stored in an interleaved manner aiding speed of vertex processing.
	*/

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) (sizeof(vec3) * 2));	// Note offset - legacy of older code
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesPCN), (GLvoid *) (sizeof(vec3) * 1));	// Same
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//if (this->normal_vertices.size() > 0)
	//{
	//	if (!this->PostGLInitialize(&this->normal_array_handle, &this->normal_coordinate_handle, this->normal_vertices.size() * sizeof(VertexAttributesP), &this->normal_vertices[0]))
	//		return false;

	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttributesP), (GLvoid *) 0);
	//	glEnableVertexAttribArray(0);
	//	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	glBindVertexArray(0);
	//}

}

void Mesh::draw(const mat4 &projection, mat4 modelview, const ivec2 &size, const float time = 0) {
	
}

Mesh::~Mesh() {
	//delete points;
	//delete trigs;
	//delete normals;
}