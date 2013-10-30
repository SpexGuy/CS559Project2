#pragma once
#include "Camera.h"
#include "Rotatable.h"

/**
 * A camera which is fixed to the surface of a sphere and points at its center
 */
class SpheroidCamera : public Camera, public SphericalCoordinateMixin{
public:
	SpheroidCamera();
	virtual glm::mat4 generateViewMatrix();
};

class FreeFlyCamera : public Camera, public SphericalAngleMixin {
protected:
	glm::vec3 position;
public:
	void moveForward(float offset);
	void moveRight(float offset);
	void moveUp(float offset);
	void setPosition(glm::vec3 position);

	virtual glm::mat4 generateViewMatrix();
};

class MarsCamera : public Camera, public RotatableMixin {
private:
	MarsCamera();
protected:
	float radius;
public:
	MarsCamera(float radius) : radius(radius) {}

	virtual glm::mat4 generateViewMatrix();
};