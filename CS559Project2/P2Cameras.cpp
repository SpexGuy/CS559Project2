#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "Camera.h"
#include "P2Cameras.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

SpheroidCamera::SpheroidCamera() {
	radius = 1.0f;
}

mat4 SpheroidCamera::generateViewMatrix() {
	return lookAt(
		vec3(radius * sin(axisAngle * M_PI / 180.0f) * sin(angle * M_PI / 180.0f),
			 radius * cos(axisAngle * M_PI / 180.0f),
			 radius * sin(axisAngle * M_PI / 180.0f) * cos(angle * M_PI / 180.0f)),
		vec3(0.0f,0.0f,0.0f),
		vec3(0.0f,1.0f,0.0f)
	);
}

void SpheroidCamera::moveForward(float offset){
	//empty method, camera is kept at constant radius
}

void SpheroidCamera::moveRight(float offset){
	float turn = offset*1.0f;
	this->addAngle(turn);
}

void SpheroidCamera::moveUp(float offset){
	float turn = offset*1.0f;
	this->addAxisAngle(-turn);
}

mat4 PointCamera::generateViewMatrix() {
	return lookAt(
		vec3(0.0f,0.0f,0.0f),
		vec3(sin(axisAngle * M_PI / 180.0f) * sin(angle * M_PI / 180.0f),
			 cos(axisAngle * M_PI / 180.0f),
			 sin(axisAngle * M_PI / 180.0f) * cos(angle * M_PI / 180.0f)),
		vec3(0.0f,1.0f,0.0f)
	);
}

void PointCamera::moveForward(float offset){
}

void PointCamera::moveRight(float offset){
	float turn = offset;
	this->addAngle(-turn);
}

void PointCamera::moveUp(float offset){
	float turn = offset*0.5f;
	this->addAxisAngle(-turn);
}


void FreeFlyCamera::moveForward(float offset) {
	float trueOffset = offset * 0.05f;
	position.y += trueOffset;
}

void FreeFlyCamera::moveRight(float offset) {
	float trueOffset = offset * 0.05f;
	position.x += trueOffset * sin((angle-90.0f) * ((float)M_PI) / 180.0f);
	position.z += trueOffset * cos((angle-90.0f) * ((float)M_PI) / 180.0f);
}

void FreeFlyCamera::moveUp(float offset) {
	float trueOffset = offset * 0.05f;
	position.x += trueOffset * sin(angle * ((float)M_PI) / 180.0f);
	position.z += trueOffset * cos(angle * ((float)M_PI) / 180.0f);
}

void FreeFlyCamera::setPosition(vec3 position) {
	this->position = position;
}

mat4 FreeFlyCamera::generateViewMatrix() {
	return lookAt(
		position,
		position + vec3(
			 sin(angle * M_PI / 180.0f) * sin(axisAngle * M_PI / 180.0f),
			 cos(axisAngle * M_PI / 180.0f),
			 cos(angle * M_PI / 180.0f) * sin(axisAngle * M_PI / 180.0f)),
		vec3(0.0f, 1.0f, 0.0f));
}

void BoundedSpheroidCamera::moveForward(float offset){
}

BoundedSpheroidCamera::BoundedSpheroidCamera(float angleBound, float angleAxisBound){
	this->angleBound = angleBound;
	this->angleAxisBound = angleAxisBound;

}

void BoundedSpheroidCamera::moveRight(float offset){
	float turn = offset;
	float a = angle;
	
	if(((angle +turn) > -180.0f && (angle+ turn)< angleBound+90.0f) || angleBound == 360.0f)
	{
		this->addAngle(turn);
	}
}

void BoundedSpheroidCamera::moveUp(float offset){
	float turn = - offset;
	float bound = 25.0f;
	float a = angle;
	if(((axisAngle +turn) > -180.0f && (axisAngle+ turn)< angleAxisBound+90.0f) || angleAxisBound == 360.f)
	{
		this->addAxisAngle(turn);
	}
}