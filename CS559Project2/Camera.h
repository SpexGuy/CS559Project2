#pragma once
#include <glm/glm.hpp>
#include "Function.h"

class AnimationGroup;

/** 
 * A camera represents a projection and a positon, and therefore
 * is responsible for generating a projection matrix and a view matrix.
 */
class Camera {
public:
	/* pushes a Rotation onto the decorator stack 
	 * Returns a pointer to the base of the stack */
	virtual Camera *rotated(const glm::vec3 &axis, const float &angle);
	
	/* pushes a Translation onto the decorator stack
	 * Returns a pointer to the base of the stack */
	virtual Camera *translated(const glm::vec3 &position);
	
	/* pushes a Scale onto the decorator stack
	 * Returns a pointer to the base of the stack */
	virtual Camera *scaled(const glm::vec3 &scale);

	/* pushes a Rotation onto the decorator stack, then adds
	 * an animation to the given group
	 * Returns a pointer to the base of the stack */
	virtual Camera *animateRotation(AnimationGroup *ag, TimeFunction<glm::vec3> *axis, TimeFunction<float> *angle);

	/* stores the current top of the decorator stack in the bucket */
	virtual Camera *store(Camera *&bucket);
	
	virtual glm::mat4 generateViewMatrix() = 0;

	void virtual moveForward(float offset) = 0;
	void virtual moveRight(float offset) = 0;
	void virtual moveUp(float offset) = 0;

	virtual ~Camera() {}
};

class CameraDecorator : public Camera {
private:
	CameraDecorator();
protected:
	Camera *next;
	bool isTos;
public:
	CameraDecorator(Camera *next) :
		next(next),
		isTos(true)
	{}
		/* pushes a Rotation onto the decorator stack 
	 * Returns a pointer to the base of the stack */
	virtual Camera *rotated(const glm::vec3 &axis, const float &angle);
	
	/* pushes a Translation onto the decorator stack
	 * Returns a pointer to the base of the stack */
	virtual Camera *translated(const glm::vec3 &position);
	
	/* pushes a Scale onto the decorator stack
	 * Returns a pointer to the base of the stack */
	virtual Camera *scaled(const glm::vec3 &scale);

	/* pushes a Rotation onto the decorator stack, then adds
	 * an animation to the given group
	 * Returns a pointer to the base of the stack */
	virtual Camera *animateRotation(AnimationGroup *ag, TimeFunction<glm::vec3> *axis, TimeFunction<float> *angle);

	/* stores the current top of the decorator stack in the bucket */
	virtual Camera *store(Camera *&bucket);

	/*Inherited methods*/
	void moveForward(float offset);
	void moveRight(float offset);
	void moveUp(float offset);
	
	virtual ~CameraDecorator();
};

/**
 * A camera which uses a Projection to generate the projection matrix
 */
class MoveableCamera : public Camera {
public:
	void virtual moveForward(float offset) = 0;
	void virtual moveRight(float offset) = 0;
	void virtual moveUp(float offset) = 0;

};