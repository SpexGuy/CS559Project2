#include <iostream>
#include <vector>
#include "Graphics.h"
#include "ErrorCheck.h"
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#ifndef	BAD_GL_VALUE
#define	BAD_GL_VALUE	GLuint(-1)
#endif

using namespace glm;
using namespace std;

Graphics Graphics::instance;
Graphics *Graphics::inst() {
	return &instance;
}

Graphics::Graphics() {
	this->projection = mat4(1.0f);
	this->view = mat4(1.0f);
	this->light = vec3(0.0f);
	this->size = ivec2(1);
}

bool Graphics::initialize() {
	return true;
}

bool Graphics::loadBuffer(GLuint *arrayHandle, GLuint *coordinateHandle, GLsizeiptr sz, const GLvoid *ptr) {
	glGenVertexArrays(1, arrayHandle);
	glBindVertexArray(*arrayHandle);
	glGenBuffers(1, coordinateHandle);
	glBindBuffer(GL_ARRAY_BUFFER, *coordinateHandle);
	glBufferData(GL_ARRAY_BUFFER, sz, ptr, GL_STATIC_DRAW);
	return !checkError("Graphics::loadBuffer - on exit");
}

void Graphics::drawWireCube() const {
	glutWireCube(2.0f);
}

void Graphics::viewport(int x, int y, int width, int height) {
	this->setSize(ivec2(width, height));
	glViewport(x, y, width, height);
}

void Graphics::drawText2D(const mat4 &base, float x, float y, char *str, float size) const {
	mat4 pos = translate(base, vec3(x, y, 0));
	float scaleFactor = size/72.0f;
	pos = scale(pos, vec3(scaleFactor, scaleFactor, 1.0f));
	glLoadMatrixf(value_ptr(pos));
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)str);
}

void Graphics::drawTriangles(const vector<ivec3> &trigs, const GLuint &vertexArrayHandle,
							 const Shader *s, const mat4 &model) const {

	const float time = 0;
								 
	mat4 modelview = view * model;
	vec3 light_pos = vec3(view * vec4(light,1.0f)); 
	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));

	s->use();
	checkError("Graphics::draw - after use");
	s->commonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm), value_ptr(light_pos));
	checkError("Graphics::draw - after common setup");

	glBindVertexArray(vertexArrayHandle);
	glDrawElements(GL_TRIANGLES , trigs.size()*3, GL_UNSIGNED_INT , &trigs[0]);
	glBindVertexArray(0);

	glUseProgram(0);

}

void Graphics::drawLines(const vector<ivec2> &segs, const GLuint &vertexArrayHandle,
						 const Shader *s, const mat4 &model) const {

	const float time = 0;
								 
	mat4 modelview = view * model;
	vec3 light_pos = vec3(view * vec4(light,1.0f)); 
	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));

	s->use();
	checkError("Graphics::draw - after use");
	s->commonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm), NULL);
	checkError("Top::Draw - after common setup");

	glBindVertexArray(vertexArrayHandle);
	glDrawElements(GL_LINES , segs.size()*2, GL_UNSIGNED_INT , &segs[0]);
	glBindVertexArray(0);

	checkError("Mesh::draw - after normals draw");
	glUseProgram(0);
}

void Graphics::takeDown() {

}

Graphics::~Graphics() {
}
