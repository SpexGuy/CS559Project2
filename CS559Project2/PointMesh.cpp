#define _USE_MATH_DEFINES
#include <math.h>
#include "PointMesh.h"
#include "Vertex.h"

using namespace std;
using namespace glm;

PointMesh::PointMesh(const vector<vec3> &points) : points(points), indexMask(points.size()) {
	this->vertex_array_handle = this->vertex_coordinate_handle = GLuint(-1);
	//indexMask is the list of indexes to be drawn.
	//we initialize it to contain all indexes
	for (uint c = 0; c < points.size(); c++) {
		indexMask[c] = c;
	}
}

bool PointMesh::initialize() {
	if (!Graphics::inst()->loadBuffer(&this->vertex_array_handle, &this->vertex_coordinate_handle, this->points.size() * sizeof(vec3), &this->points[0]))
		return false;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid *) 0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	this->starShader = ShaderFlyweight::inst()->getShader(SHADER_STAR);

	return true;
}

void PointMesh::draw(const mat4 &model) {

	Graphics::inst()->drawPoints(
		indexMask,
		vertex_array_handle,
		starShader,
		model);

}

void PointMesh::takeDown() {
	if (this->vertex_array_handle != GLuint(-1))
		glDeleteVertexArrays(1, &this->vertex_array_handle);

	if (this->vertex_coordinate_handle != GLuint(-1))
		glDeleteBuffers(1, &this->vertex_coordinate_handle);

	this->vertex_array_handle = this->vertex_coordinate_handle = GLuint(-1);
}

PointMesh::~PointMesh() {
	//assert that takeDown has been called
	assert(this->vertex_array_handle == GLuint(-1));
	assert(this->vertex_coordinate_handle == GLuint(-1));
}






PointMesh *PointMesh::newStarField(const int &numPoints, const float &radius) {
	vector<vec3> points(numPoints);
	for (uint c = 0; c < points.size(); c++) {
		/** picking random points on a sphere is a nontrivial
		 * problem. If a random phi and theta are picked, 
		 * you will see clustering around the poles.
		 * This solution comes from
		 * http://mathworld.wolfram.com/SpherePointPicking.html */
		float cosphi = 2*float(rand())/(float(RAND_MAX)) - 1.0f;
		float sinphi = sign(cosphi) * sqrt(1 - cosphi*cosphi);
		float theta = float(2*M_PI * float(rand())/(float(RAND_MAX)));

		points[c] = vec3(radius * sinphi * sin(theta),
								  radius * cosphi,
								  radius * sinphi * cos(theta));

	}

	return new PointMesh(points);
}
