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
	void moveForward(float offset);
	void moveRight(float offset);
	void moveUp(float offset);
};

/**
 * A camera which is fixed to the surface of a sphere and points at its center
 * but is internally bounded to phi and a theta in the move functions. 
 */
class BoundedSpheroidCamera : public SpheroidCamera
{
public:
	BoundedSpheroidCamera(float angleBound, float angleAxisBound);

	void moveForward(float offset);
	void moveRight(float offset);
	void moveUp(float offset);

private:
	float angleBound;
	float angleAxisBound;
};

/**
 * A camera which rotates around its self
 */
class PointCamera : public Camera, public SphericalAngleMixin {
public:
	virtual glm::mat4 generateViewMatrix();
	void moveForward(float offset);
	void moveRight(float offset);
	void moveUp(float offset);
};

/**
 * A camera that is bound to no point and has a movable postion
 */
class FreeFlyCamera : public Camera, public SphericalAngleMixin {
protected:
	glm::vec3 position;
public:
	FreeFlyCamera() : position(glm::vec3(0.0f)) {}

	void moveForward(float offset);
	void moveRight(float offset);
	void moveUp(float offset);

	void reset();
	
	void setPosition(glm::vec3 position);

	virtual glm::mat4 generateViewMatrix();
};
