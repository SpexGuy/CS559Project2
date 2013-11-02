#pragma once
#include "Camera.h"
#include "Rotatable.h"

/**
 * A camera which is fixed to the surface of a sphere and points at its center
 */
class SpheroidCamera : public MoveableCamera, public SphericalCoordinateMixin{
public:
	SpheroidCamera();
	virtual glm::mat4 generateViewMatrix();
	void moveForward(float offset);
	void moveRight(float offset);
	void moveUp(float offset);
};

class BoundedSpheroidCamera : public SpheroidCamera
{
public:
	BoundedSpheroidCamera();

	void moveForward(float offset);
	void moveRight(float offset);
	void moveUp(float offset);

private:
	float bound;
	float pi;
	float theta;
};

class PointCamera : public MoveableCamera, public SphericalAngleMixin {
public:
	virtual glm::mat4 generateViewMatrix();
	void moveForward(float offset);
	void moveRight(float offset);
	void moveUp(float offset);
};

/**
 * A camera that is bound to no point and has a movable postion
 */
class FreeFlyCamera : public MoveableCamera, public SphericalAngleMixin {
protected:
	glm::vec3 position;
public:
	FreeFlyCamera() : MoveableCamera() {}

	void moveForward(float offset);
	void moveRight(float offset);
	void moveUp(float offset);

	void reset();
	
	void setPosition(glm::vec3 position);

	virtual glm::mat4 generateViewMatrix();
};

/**
 * MarsCamera is a special camera that can be animated to rotate around
 * a point at a certain radius 
 */
class MarsCamera : public MoveableCamera, public RotatableMixin {
private:
		MarsCamera();
protected:
	float radius;
public:
	MarsCamera(float radius) : MoveableCamera(), radius(radius) {}
	
	void moveForward(float offset);
	void moveRight(float offset);
	void moveUp(float offset);

	virtual glm::mat4 generateViewMatrix();
};
