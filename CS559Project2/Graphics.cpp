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

GLsizei Graphics::numHandles = 4;

const Graphics Graphics::inst;

Graphics::Graphics() {
	this->sphereHandle = BAD_GL_VALUE;
	this->wireCubeHandle = BAD_GL_VALUE;
	this->wireSphereHandle = BAD_GL_VALUE;
	this->cylinderHandle = BAD_GL_VALUE;
}

bool Graphics::initialize() {
	if (sphereHandle == BAD_GL_VALUE) {
		GLUquadric * q = gluNewQuadric();
		if (q != NULL) {
			if ((sphereHandle = glGenLists(numHandles)) == 0) {
				cout << "Graphics::initialize() - Failed to GenLists()" << endl;
				return false;
			}
			glNewList(sphereHandle, GL_COMPILE);
			gluSphere(q, 1.0, 32, 32);
			glEndList();
			
			wireCubeHandle = sphereHandle + 1;
			glNewList(wireCubeHandle, GL_COMPILE);
			glutWireCube(2.0f);
			glEndList();

			wireSphereHandle = sphereHandle + 2;
			glNewList(wireSphereHandle, GL_COMPILE);
			glutWireSphere(1.0f, 32, 32);
			glEndList();

			cylinderHandle = sphereHandle + 3;
			glNewList(cylinderHandle, GL_COMPILE);
			gluCylinder(q, 1.0, 1.0, 1.0, 32, 32);
			glEndList();

			gluDeleteQuadric(q);
		} else {
			return false;
		}
	}
	return true;
}

void Graphics::drawSphere() const {
	glCallList(sphereHandle);
}

void Graphics::drawWireCube() const {
	glCallList(wireCubeHandle);
}

void Graphics::drawWireSphere() const {
	glCallList(wireSphereHandle);
}

void Graphics::drawCylinder() const {
	glCallList(cylinderHandle);
}

void Graphics::drawText2D(const mat4 &base, float x, float y, char *str, float size) const {
	mat4 pos = translate(base, vec3(x, y, 0));
	float scaleFactor = size/72.0f;
	pos = scale(pos, vec3(scaleFactor, scaleFactor, 1.0f));
	glLoadMatrixf(value_ptr(pos));
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)str);
}

Graphics::~Graphics() {
	if (sphereHandle != BAD_GL_VALUE) {
		glDeleteLists(sphereHandle, numHandles);
	}
}