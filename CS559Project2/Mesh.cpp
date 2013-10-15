#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "Mesh.h"
#include "shader.h"
#include "object.h"
#include "Vertex.h"
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

#define DEBUG

//int main() {
//	Mesh::newMars(10, 1, "mars_low_rez.txt");
//	cout << "worked!" << endl;
//	int i;
//	cin >> i;
//	return 0;
//}

Mesh *Mesh::newMars(float radius, float radScale, char *filename, bool crosshatch){
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

		return Mesh::newMars(radius, radScale, radii, crosshatch);
	} else {
		cerr << "Could not open file '" << filename << "'!" << endl;
		assert(false);
		return NULL;
	}
}

Mesh *Mesh::newMars(float radius, float radScale,
					vector<vector<float>> radii, bool crosshatch)
{
	//Assumes that the 2D has the same amount of slices for each stack
	//Slices are the columns of the mesh

	int height = radii.size();
	assert(height > 0);
	int width = radii[0].size();
	assert(width > 0);

	vector<vec3> points/*(height*width + 2)*/;

	for(int i = 0; i< height; i++) {
		for( int j = 0; j < width; j++) {
			float r = radius + radScale*radii[i][j];
			float theta = float(2*M_PI * float(j) / float(width));
			float phi = float(M_PI * float(i+1) / float(height+1));

			float x = r*sin(theta)*sin(phi);
			float z = r*cos(theta)*sin(phi);
			float y = r*cos(phi);

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
	points.push_back(vec3(0, radius, 0));
	points.push_back(vec3(0, -radius, 0));

	vector<ivec3> trigs = generateTrigs(points, width, height, true, crosshatch);

	return new Mesh(points, trigs);
}

vector<ivec3> Mesh::generateTrigs(vector<vec3> points, int width, int height, bool endcaps, bool crosshatch) {
	assert(points.size() >= width*height + (endcaps ? 2 : 0));
	vector<ivec3> trigs;
	for (int c = 0; c < height-1; c++) {
		for (int d = 0; d < width; d++) {
			int tlIndex = c*width + d;
			int blIndex = tlIndex + width;
			int trIndex = c*width + (d+1)%width;
			int brIndex = trIndex + width;
			if (crosshatch && ((c%2 == 0) ^ (d%2 == 0))) {
				trigs.push_back(ivec3(tlIndex, blIndex, brIndex));
				trigs.push_back(ivec3(brIndex, trIndex, tlIndex));
			} else {
				trigs.push_back(ivec3(trIndex, tlIndex, blIndex));
				trigs.push_back(ivec3(blIndex, brIndex, trIndex));
			}

#ifdef DEBUG
			if(c < 1 && (d > width - 1 || d < 1))
			cout << "( " << trIndex << ", " << tlIndex << "," << blIndex << " )" <<endl;
			if(c < 1 && (d > width - 1 || d < 1))
			cout << "( " << blIndex << ", " << brIndex << "," << trIndex << " )" <<endl<<endl;
#endif
		}
	}

	if (endcaps) {
		int botRow = (height-1)*width;
		for (int c = 0; c < width; c++) {
			trigs.push_back(ivec3(c, (c+1)%width, points.size()-2));
			trigs.push_back(ivec3(points.size()-1, botRow + (c+1)%width, botRow + c));
		}
	}

	return trigs;
}





Mesh::Mesh(vector<vec3> ppoints, vector<ivec3> trigs) {
	this->points = vector<VertexPN>(ppoints.size());
	this->trigs = trigs;

	for (int c = 0; c < points.size(); c++) {
		points[c].position = ppoints[c];
		points[c].normal = vec3(0.0f);
		points[c].light_position = vec3( 0, 5.0f, 0.0f);
	
	}

	for (int c = 0; c < trigs.size(); c++) {
		vec3 vect1 = ppoints[trigs[c].y] - ppoints[trigs[c].z];
		vec3 vect2 = ppoints[trigs[c].y] - ppoints[trigs[c].x];
		vec3 planeNormal = cross(vect1, vect2);

		points[trigs[c].x].normal += planeNormal;
		points[trigs[c].y].normal += planeNormal;
		points[trigs[c].z].normal += planeNormal;
	}
	for (int c = 0; c < points.size(); c++) {
		points[c].normal = normalize(points[c].normal);
	}

	for (int c = 0; c < points.size(); c++) {
		normPoints.push_back(points[c].position);
		normPoints.push_back(points[c].position + vec3(0.1f)*points[c].normal);
		normSegs.push_back(ivec2(2*c, 2*c+1));
	}
}



bool Mesh::initialize() {
	if (!this->PostGLInitialize(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->points.size() * sizeof(VertexPN), &this->points[0]))
		return false;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (GLvoid *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (GLvoid *) (1*sizeof(vec3)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (GLvoid *) (2*sizeof(vec3)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if (this->normPoints.size() > 0) {
		if (!this->PostGLInitialize(&this->normal_array_handle, &this->normal_coordinate_handle, this->normPoints.size() * sizeof(vec3), &this->normPoints[0]))
			return false;

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid *) 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	
	if (!this->solidShader.Initialize("solid_shader.vert", "solid_shader.frag"))
		return false;
	if (!this->shader.Initialize("top_shader.vert", "top_shader.frag"))
		return false;

	return true;
}

void Mesh::draw(mat4 model) const {
	bool normals = false;
	if (this->GLReturnedError("Mesh::Draw - on entry"))
		return;

	transform(model);

	glEnable(GL_DEPTH_TEST);

	model = rotate(model, 30.0f, vec3(0.0f, 1.0f, 0.0f));

	Graphics::inst()->drawTriangles(trigs, vertex_array_handle, shader, model);

	//mat4 modelview = view * model;
	//vec3 light_pos = vec3(view * vec4(light,1.0f)); 
	//mat4 mvp = projection * modelview;
	//mat3 nm = inverse(transpose(mat3(modelview)));

	//this->shader.Use();
	//this->GLReturnedError("Top::Draw - after use");
	//this->shader.CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm), value_ptr(light_pos));
	//this->GLReturnedError("Top::Draw - after common setup");

	//glBindVertexArray(this->vertex_array_handle);
	//glDrawElements(GL_TRIANGLES , this->trigs.size()*3, GL_UNSIGNED_INT , &this->trigs[0]);
	//glBindVertexArray(0);

	//this->GLReturnedError("Mesh::draw - after draw");

	//glUseProgram(0);

	//drawing normals
	//if(normals) {
	//	this->solidShader.Use();
	//	this->GLReturnedError("Top::Draw - after use");
	//	this->solidShader.CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm), NULL);
	//	this->GLReturnedError("Top::Draw - after common setup");
	//
	//	glBindVertexArray(this->normal_array_handle);
	//	glDrawElements(GL_LINES , this->normSegs.size()*2, GL_UNSIGNED_INT , &this->normSegs[0]);
	//	glBindVertexArray(0);

	//	this->GLReturnedError("Mesh::draw - after normals draw");
	//	glUseProgram(0);
	//	if (this->GLReturnedError("Mesh::draw - on exit"))
	//	return;
	//}
}

Mesh::~Mesh() {
	//delete points;
	//delete trigs;
	//delete normals;
}