#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

SpheroidCamera::SpheroidCamera(Projection *p) :
	DynamicProjectionCamera(p) {
	radius = 1.0f;
}

mat4 SpheroidCamera::generateViewMatrix() {
	return lookAt(
		vec3(radius * sin(axisAngle * M_PI / 180.0f) * sin(angle * M_PI / 180.0f),
			 radius * cos(axisAngle * M_PI / 180.0f),
			 radius * sin(axisAngle * M_PI / 180.0f) * cos(angle * M_PI / 180.0f)),
		vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f)
	);
}


void FreeFlyCamera::moveForward(float offset) {
	position.x += offset * sin(angle * M_PI / 180.0f);
	position.z += offset * cos(angle * M_PI / 180.0f);
}

void FreeFlyCamera::moveRight(float offset) {
	position.z += offset * sin(angle * M_PI / 180.0f);
	position.x += offset * cos(angle * M_PI / 180.0f);
}

void FreeFlyCamera::moveUp(float offset) {
	position.y += offset;
}

void FreeFlyCamera::setPosition(vec3 position) {
	this->position = position;
}

mat4 FreeFlyCamera::generateViewMatrix() {
	return lookAt(
		vec3(0.0f, 0.0f, 0.0f),
		vec3(sin(angle * M_PI / 180.0f) * sin(axisAngle * M_PI / 180.0f),
			 cos(axisAngle * M_PI / 180.0f),
			 cos(angle * M_PI / 180.0f) * sin(axisAngle * M_PI / 180.0f)),
		vec3(0.0f, 1.0f, 0.0f));
}