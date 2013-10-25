#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "ErrorCheck.h"
#include "Mesh.h"
#include "Shader.h"
#include "Vertex.h"
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

#define DEBUG

Mesh *Mesh::newMars(float radius, float radScale, char *filename, bool crosshatch) {
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

Mesh *Mesh::newSphere(int stacks, int slices, float radius, bool crosshatch)
{
	//Assumes that the 2D has the same amount of slices for each stack
	//Slices are the columns of the mesh

	int height = stacks;
	assert(height > 0);
	int width = slices;
	assert(width > 0);

	

	vector<vec3> points/*(height*width + 2)*/;
	for(int i = 0; i< height; i++) {
		for( int j = 0; j < width; j++) {
			float theta = float(2*M_PI * float(j) / float(width));
			float phi = float(M_PI * float(i+1) / float(height+1));

			float x = radius*sin(theta)*sin(phi);
			float z = radius*cos(theta)*sin(phi);
			float y = radius*cos(phi);

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

Mesh *Mesh::newCylinder(int stacks, int slices, float tall, float topRadius, float botRadius, bool crosshatch)
{
	//Assumes that the 2D has the same amount of slices for each stack
	//Slices are the columns of the mesh

	int height = stacks;
	assert(height > 0);
	int width = slices;
	assert(width > 0);

	vector<vec3> points/*(height*width + 2)*/;
	float ratio = abs(topRadius-botRadius)/tall;

	for(int i = 0; i< height; i++) {
		for( int j = 0; j < width; j++) {
			float r = ((float)i*ratio) + (float)botRadius;
			float theta = float(2*M_PI * float(j) / float(width));

			float z = r*sin(theta);
			float x = r*cos(theta);
			float y = float(i)/float(height);

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

	vector<ivec3> trigs = generateTrigs(points, width, height, false, crosshatch);

	return new Mesh(points, trigs);
}

Mesh *Mesh::newSurfaceOfRotation(const vector<vec2> &points, int slices, bool crosshatch) {
	vector<vec3> verts;
	for (uint c = 1; c < points.size()-1; c++) {
		for (int d = 0; d < slices; d++) {
			verts.push_back(
				vec3(points[c].y * cos(2*M_PI * float(d)/slices), 
					 points[c].x,
					 points[c].y * sin(2*M_PI * float(d)/slices)));
		}
	}
	verts.push_back(vec3(0.0f, points[0].x, 0.0f));
	verts.push_back(vec3(0.0f, points[points.size()-1].x, 0.0f));

	vector<ivec3> trigs = generateTrigs(verts, slices, points.size()-2, true, crosshatch);

	return new Mesh(verts, trigs);
}

vector<ivec3> Mesh::generateTrigs(vector<vec3> points, int width, int height, bool endcaps, bool crosshatch) {
	assert(int(points.size()) >= width*height + (endcaps ? 2 : 0));
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
	this->drawNormals = false;
	this->vertex_array_handle = this->vertex_coordinate_handle = GLuint(-1);
	this->normal_array_handle = this->normal_coordinate_handle = GLuint(-1);
	this->points = vector<VertexPN>(ppoints.size());
	this->trigs = trigs;

	for (int c = 0; c < int(points.size()); c++) {
		points[c].position = ppoints[c];
		points[c].normal = vec3(0.0f);
		points[c].light_position = vec3( 0, 5.0f, 0.0f);
	}

	for (int c = 0; c < int(trigs.size()); c++) {
		vec3 vect1 = ppoints[trigs[c].y] - ppoints[trigs[c].z];
		vec3 vect2 = ppoints[trigs[c].y] - ppoints[trigs[c].x];
		vec3 planeNormal = cross(vect1, vect2);

		points[trigs[c].x].normal += planeNormal;
		points[trigs[c].y].normal += planeNormal;
		points[trigs[c].z].normal += planeNormal;
	}
	for (int c = 0; c < int(points.size()); c++) {
		if (points[c].normal == vec3(0.0f, 0.0f, 0.0f))
			points[c].normal = vec3(0.0f, 1.0f, 0.0f);
		points[c].normal = normalize(points[c].normal);
	}

	for (int c = 0; c < int(points.size()); c++) {
		normPoints.push_back(points[c].position);
		normPoints.push_back(points[c].position + vec3(0.1f)*points[c].normal);
		normSegs.push_back(ivec2(2*c, 2*c+1));
	}
}

bool Mesh::initialize() {
	if (!Graphics::inst()->loadBuffer(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->points.size() * sizeof(VertexPN), &this->points[0]))
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
		if (!Graphics::inst()->loadBuffer(&this->normal_array_handle, &this->normal_coordinate_handle, this->normPoints.size() * sizeof(vec3), &this->normPoints[0]))
			return false;

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid *) 0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	this->solidShader = ShaderFlyweight::inst()->getShader(SHADER_SOLID);
	this->shader = ShaderFlyweight::inst()->getShader(SHADER_ADS);

	return true;
}

void Mesh::draw(mat4 model) {
	if (checkError("Mesh::Draw - on entry"))
		return;

	transform(model);

	Graphics::inst()->drawTriangles(trigs, vertex_array_handle, shader, model);

	if(drawNormals) {
		Graphics::inst()->drawLines(normSegs, normal_array_handle, solidShader, model);
		if (checkError("Mesh::draw - on exit"))
			return;
	}
}

/*	Notice the destructor in this case asserts that all resources
	that don't go away by themselves have ALREADY been released. This
	is because the destructor might be called after a GL context has
	been destroyed, so I force the user of this class to call the
	TakeDown() purposefully.
*/
Mesh::~Mesh() {
	assert(this->vertex_array_handle == GLuint(-1));
	assert(this->vertex_coordinate_handle == GLuint(-1));
	assert(this->normal_array_handle == GLuint(-1));
	assert(this->normal_coordinate_handle == GLuint(-1));
}

void Mesh::takeDown() {
	if (this->vertex_array_handle != GLuint(-1))
		glDeleteVertexArrays(1, &this->vertex_array_handle);

	if (this->vertex_coordinate_handle != GLuint(-1))
		glDeleteBuffers(1, &this->vertex_coordinate_handle);

	if (this->normal_array_handle != GLuint(-1))
		glDeleteVertexArrays(1, &this->normal_array_handle);

	if (this->normal_coordinate_handle != GLuint(-1))
		glDeleteBuffers(1, &this->normal_coordinate_handle);

	this->vertex_array_handle = this->vertex_coordinate_handle = GLuint(-1);
	this->normal_array_handle = this->normal_coordinate_handle = GLuint(-1);

}


