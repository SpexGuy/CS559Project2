#define _USE_MATH_DEFINES
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Rotatable.h"
#include <iostream>

using namespace glm;
using namespace std;

RotatableMixin::RotatableMixin() {
	this->axis = vec3(0.0f, 1.0f, 0.0f);
	this->angle = 0.0f;
}

void RotatableMixin::transform(glm::mat4 & context) const {
	context = rotate(context, angle, axis);
}

void RotatableMixin::setRotation(vec3 & axis, float angle) {
	this->axis = axis;
	this->angle = angle;
}



SphericalCoordinateMixin::SphericalCoordinateMixin() {
	this->angle = 0.0f;
	this->axisAngle = 90.0f;
}

void SphericalCoordinateMixin::setAngle(float angle) {
	this->angle = float(fmod(angle, 360.0f));
}

void SphericalCoordinateMixin::addAngle(float offset) {
	setAngle(angle+offset);
}

void SphericalCoordinateMixin::setAxisAngle(float angle) {
	// force angle into range [1, 179]
	if (angle < 1)
		angle = 1.0f;
	if (angle > 179)
		angle = 179.0f;
	this->axisAngle = angle;
}

void SphericalCoordinateMixin::addAxisAngle(float offset) {
	setAxisAngle(axisAngle+offset);
}



void SphericalRotationMixin::transform(mat4 & context) const {
	context = glm::rotate(context, angle, vec3(0.0f, 1.0f, 0.0f));
	//since the context has been rotated, (1, 0, 0) is now
	//perpendicular to the plane on which we want to rotate
	context = glm::rotate(context, axisAngle, vec3(1.0f, 0.0f, 0.0f));
}
