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
	float turn = offset*0.5f;
	this->addAngle(-turn);
}

void PointCamera::moveUp(float offset){
	float turn = offset*0.5f;
	this->addAxisAngle(-turn);
}


void FreeFlyCamera::moveForward(float offset) {
	float trueOffset = offset * 0.05f;
	position.x += trueOffset * sin(angle * ((float)M_PI) / 180.0f);
	position.z += trueOffset * cos(angle * ((float)M_PI) / 180.0f);
}

void FreeFlyCamera::moveRight(float offset) {
	float trueOffset = offset * 0.05f;
	position.x += trueOffset * sin((angle-90.0f) * ((float)M_PI) / 180.0f);
	position.z += trueOffset * cos((angle-90.0f) * ((float)M_PI) / 180.0f);
}

void FreeFlyCamera::moveUp(float offset) {
	float trueOffset = offset * 0.05f;
	position.y += trueOffset;
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

mat4 MarsCamera::generateViewMatrix()
{
	mat4 view =  lookAt(
		vec3(radius, 0.0f, 0.0f),
		vec3(-radius, 0.0f, 2.0f),
		vec3(1.0f, 0.0f, 0.0f));
	return glm::rotate(view, this->angle, this->axis);
}

void MarsCamera::moveForward(float offset){
	//empty method, camera is kept at constant radius
}

void MarsCamera::moveRight(float offset){

}

void MarsCamera::moveUp(float offset){
	
}