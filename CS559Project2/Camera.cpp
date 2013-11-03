/** See Camera.h for architecture documentation */
#include "Camera.h"
#include "Rotatable.h"
#include "Translatable.h"
#include "Scaleable.h"
#include "Animation.h"

using namespace glm;


//---------------- Camera -----------------

Camera *Camera::pushDecorator(CameraDecorator *d) {
	d->setNext(this);
	return d;
}

Camera *Camera::store(Camera *&bucket) {
	bucket = this;
	return this;
}

Camera *Camera::rotated(const vec3 &axis, const float &angle) {
	CamRotation *rot = new CamRotation();
	rot->setRotation(axis, angle);
	return pushDecorator(rot);
}

Camera *Camera::translated(const vec3 &position) {
	CamTranslation *trans = new CamTranslation();
	trans->position(position);
	return pushDecorator(trans);
}

Camera *Camera::scaled(const vec3 &scale) {
	CamScale *sc = new CamScale();
	sc->scale(scale);
	return pushDecorator(sc);
}

Camera *Camera::animateRotation(AnimationGroup *ag, TimeFunction<vec3> *axis, TimeFunction<float> *angle) {
	CamRotation *rot = new CamRotation();
	RotationAnimation *anim = new RotationAnimation(rot, axis, angle);
	ag->addAnimation(anim);
	return pushDecorator(rot);
}


//----------------- CameraDecorator -----------------

Camera *CameraDecorator::pushDecorator(CameraDecorator *dec) {
	Camera *d = next->pushDecorator(dec);
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

void CameraDecorator::moveForward(float offset){
	next->moveForward(offset);
}

void CameraDecorator::moveRight(float offset){
	next->moveRight(offset);
}

void CameraDecorator::moveUp(float offset){
	next->moveUp(offset);
}

