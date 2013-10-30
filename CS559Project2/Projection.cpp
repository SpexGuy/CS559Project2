#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include "Projection.h"
#include "Graphics.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

Projection::Projection() {
	hither = 1.0f;
	yon = 10.0f;
}

void Projection::setPlanes(float hither, float yon) {
	if (hither <= 0)
		hither = 0.0000001f;
	if (hither >= yon)
		yon = hither + 0.0000001f;
	this->hither = hither;
	this->yon = yon;
}



OrthogonalProjection::OrthogonalProjection(float height) {
	setHeight(height);
}

void OrthogonalProjection::setHeight(float height) {
	this->height = height;
}

mat4 OrthogonalProjection::generateProjectionMatrix() {
	ivec2 size = Graphics::inst()->getSize();
	float aspect = float(size.x)/float(size.y);
	if (size.x > size.y) {
		return ortho(-aspect*height, aspect*height, -height, height, hither, yon);
	} else {
		return ortho(-height, height, -height/aspect, height/aspect, hither, yon);
	}
}



PerspectiveProjection::PerspectiveProjection(float fov) {
	this->fov = fov;
}

mat4 PerspectiveProjection::generateProjectionMatrix() {
	ivec2 size = Graphics::inst()->getSize();
	return perspective(fov, float(size.x)/float(size.y), hither, yon);
}

void PerspectiveProjection::addFov(float addition) {
	setFov(fov + addition);
}
void PerspectiveProjection::setFov(float fov) {
	if (fov > 80)
		fov = 80;
	else if (fov < 10)
		fov = 10;
	this->fov = fov;
}
float PerspectiveProjection::getFov() {
	return fov;
}
