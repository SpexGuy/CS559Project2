#define _USE_MATH_DEFINES
#include <math.h>
#include "PointMesh.h"
#include "Vertex.h"

using namespace std;
using namespace glm;

PointMesh::PointMesh(vector<vec3> points) : points(points), indexMask(points.size()) {
	this->vertex_array_handle = this->vertex_coordinate_handle = GLuint(-1);
	for (uint c = 0; c < points.size(); c++) {
		indexMask[c] = c;
	}
}

bool PointMesh::initialize() {
	if (!Graphics::inst()->loadBuffer(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->points.size() * sizeof(vec3), &this->points[0]))
		return false;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid *) 0);
//	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPC), (GLvoid *) sizeof(vec3));

	glEnableVertexAttribArray(0);
//	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	this->starShader = ShaderFlyweight::inst()->getShader(SHADER_STAR);

	return true;
}

void PointMesh::draw(const mat4 &model) {
	mat4 m = model;
	transform(m);
	Graphics::inst()->drawPoints(indexMask, vertex_array_handle, starShader, m);
}

void PointMesh::takeDown() {
	if (this->vertex_array_handle != GLuint(-1))
		glDeleteVertexArrays(1, &this->vertex_array_handle);

	if (this->vertex_coordinate_handle != GLuint(-1))
		glDeleteBuffers(1, &this->vertex_coordinate_handle);

	this->vertex_array_handle = this->vertex_coordinate_handle = GLuint(-1);
}

PointMesh::~PointMesh() {
	assert(this->vertex_array_handle == GLuint(-1));
	assert(this->vertex_coordinate_handle == GLuint(-1));
}






PointMesh *PointMesh::newStarField(int numPoints, float radius) {
	vector<vec3> points(numPoints);
	for (uint c = 0; c < points.size(); c++) {
		float cosphi = 2*float(rand())/(float(RAND_MAX)) - 1.0f;
		float sinphi = sign(cosphi) * sqrt(1 - cosphi*cosphi);
		float theta = float(2*M_PI * float(rand())/(float(RAND_MAX)));

		points[c] = vec3(radius * sinphi * sin(theta),
								  radius * cosphi,
								  radius * sinphi * cos(theta));

	}

	return new StarField(points);
}

void StarField::draw(const mat4 &model) {
//	glDisable(GL_DEPTH_TEST);
//	mat4 oldProj = Graphics::inst()->getProjection();
//	ivec2 size = Graphics::inst()->getSize();
//	float aspect = float(size.x)/float(size.y);
//	Graphics::inst()->setProjection(
//		ortho(-aspect, aspect, -1.0f, 1.0f, 3.0f, 100.0f));
	//HACK: clips front stars by setting near plane to 3.0f (the radius of the camera)
	PointMesh::draw(model);
//	Graphics::inst()->setProjection(oldProj);
//	glEnable(GL_DEPTH_TEST);
}