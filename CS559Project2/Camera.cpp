#include "Camera.h"
#include "Rotatable.h"
#include "Translatable.h"
#include "Scaleable.h"
#include "Animation.h"

using namespace glm;

Camera *Camera::rotated(const vec3 &axis, const float &angle) {
	CamRotation *rot = new CamRotation(this);
	rot->setRotation(axis, angle);
	return rot;
}

Camera *Camera::translated(const vec3 &position) {
	CamTranslation *trans = new CamTranslation(this);
	trans->position(position);
	return trans;
}

Camera *Camera::scaled(const vec3 &scale) {
	CamScale *sc = new CamScale(this);
	sc->scale(scale);
	return sc;
}

Camera *Camera::animateRotation(AnimationGroup *ag, TimeFunction<vec3> *axis, TimeFunction<float> *angle) {
	CamRotation *rot = new CamRotation(this);
	RotationAnimation *anim = new RotationAnimation(rot, axis, angle);
	ag->addAnimation(anim);
	return rot;
}

Camera *Camera::store(Camera *&bucket) {
	bucket = this;
	return this;
}




Camera *CameraDecorator::rotated(const vec3 &axis, const float &angle) {
	Camera *d = next->rotated(axis, angle);
	if (d != next)
		isTos = false;
	next = d;
	return this;
}

Camera *CameraDecorator::translated(const vec3 &position) {
	Camera *d = next->translated(position);
	if (d != next)
		isTos = false;
	next = d;
	return this;
}

Camera *CameraDecorator::scaled(const vec3 &scale) {
	Camera *d = next->scaled(scale);
	if (d != next)
		isTos = false;
	next = d;
	return this;
}

Camera *CameraDecorator::animateRotation(AnimationGroup *ag, TimeFunction<vec3> *axis, TimeFunction<float> *angle) {
	Camera *d = next->animateRotation(ag, axis, angle);
	if (d != next)
		isTos = false;
	next = d;
	return this;
}

Camera *CameraDecorator::store(Camera *&bucket) {
	if (isTos) {
		bucket = this;
	} else {
		next->store(bucket);
	}
	return this;
}

CameraDecorator::~CameraDecorator() {
	if (next != NULL)
		delete next;
}

