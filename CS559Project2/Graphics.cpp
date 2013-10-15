#include <iostream>
#include <vector>
#include "Graphics.h"
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
}

bool Graphics::initialize() {
	return true;
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
							 const Shader &s, const mat4 &model) const {

	const float time = 0;
	vec3 light(2, 2, 0);				 
								 
	mat4 modelview = view * model;
	vec3 light_pos = vec3(view * vec4(light,1.0f)); 
	mat4 mvp = projection * modelview;
	mat3 nm = inverse(transpose(mat3(modelview)));

	s.Use();
	//this->GLReturnedError("Top::Draw - after use");
	s.CommonSetup(time, value_ptr(size), value_ptr(projection), value_ptr(modelview), value_ptr(mvp), value_ptr(nm), value_ptr(light_pos));
	//this->GLReturnedError("Top::Draw - after common setup");

	glBindVertexArray(vertexArrayHandle);
	glDrawElements(GL_TRIANGLES , trigs.size()*3, GL_UNSIGNED_INT , &trigs[0]);
	glBindVertexArray(0);

	glUseProgram(0);

}

void Graphics::takeDown() {

}

Graphics::~Graphics() {
}
