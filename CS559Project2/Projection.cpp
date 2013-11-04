#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
/* See Projection.h for architecture documentation */
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

void Projection::setPlanes(const float &hither, const float &yon) {
	this->hither = max(0.0000001f, hither);
	this->yon = max(this->hither + 0.0000001f, yon);
}



OrthogonalProjection::OrthogonalProjection(const float &height) {
	setHeight(height);
}

void OrthogonalProjection::setHeight(const float &height) {
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



PerspectiveProjection::PerspectiveProjection(const float &fov) {
	setFov(fov);
}

mat4 PerspectiveProjection::generateProjectionMatrix() {
	ivec2 size = Graphics::inst()->getSize();
	return perspective(fov, float(size.x)/float(size.y), hither, yon);
}

void PerspectiveProjection::setFov(const float &fov) {
	this->fov = clamp(fov, 10.0f, 80.0f);
}
