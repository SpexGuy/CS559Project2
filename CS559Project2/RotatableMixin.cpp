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

void RotatableMixin::transform(glm::mat4 &context) const {
	context = rotate(context, angle, axis);
}

void RotatableMixin::setRotation(vec3 &axis, float angle) {
	this->axis = axis;
	this->angle = angle;
}



void SphericalRotationMixin::transform(mat4 &context) const {
	context = glm::rotate(context, angle, vec3(0.0f, 1.0f, 0.0f));
	//since the context has been rotated, (1, 0, 0) is now
	//perpendicular to the plane on which we want to rotate
	context = glm::rotate(context, axisAngle, vec3(1.0f, 0.0f, 0.0f));
}

void SphericalPositionMixin::transform(mat4 &context) const {
	context = glm::rotate(context, angle, vec3(0.0f, 1.0f, 0.0f));
	context = glm::rotate(context, axisAngle, vec3(1.0f, 0.0f, 0.0f));
	context = glm::translate(context, vec3(0.0f, radius, 0.0f));
}