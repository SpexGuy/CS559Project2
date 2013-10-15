#include <iostream>
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
Graphics Graphics::inst() {
	return instance;
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

void Graphics::drawText2D(const mat4 &base, float x, float y, char *str, float size) const {
	mat4 pos = translate(base, vec3(x, y, 0));
	float scaleFactor = size/72.0f;
	pos = scale(pos, vec3(scaleFactor, scaleFactor, 1.0f));
	glLoadMatrixf(value_ptr(pos));
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)str);
}

void Graphics::takeDown() {

}

Graphics::~Graphics() {
}
