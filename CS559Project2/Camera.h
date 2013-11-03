#pragma once
#include <glm/glm.hpp>
#include "Function.h"

class AnimationGroup;
class CameraDecorator;

/** 
 * A camera represents a projection and a positon, and therefore
 * is responsible for generating a projection matrix and a view matrix.
 */
class Camera {
public:

	/* pushes the given Decorator onto the decorator stack 
	 * Returns a pointer to the base of the stack */
	virtual Camera *pushDecorator(CameraDecorator *d);

	/* stores the current top of the decorator stack in the bucket */
	virtual Camera *store(Camera *&bucket);
	
	virtual glm::mat4 generateViewMatrix() = 0;

	void virtual moveForward(float offset) = 0;
	void virtual moveRight(float offset) = 0;
	void virtual moveUp(float offset) = 0;

	virtual ~Camera() {}

	//--------- Decorator Functions -----------------
	//The following functions exist only as wrappers to pushDecorator
	//to enrich the syntax

	/* pushes a Rotation onto the decorator stack 
	 * Returns a pointer to the base of the stack */
	Camera *rotated(const glm::vec3 &axis, const float &angle);
	
	/* pushes a Translation onto the decorator stack
	 * Returns a pointer to the base of the stack */
	Camera *translated(const glm::vec3 &position);
	
	/* pushes a Scale onto the decorator stack
	 * Returns a pointer to the base of the stack */
	Camera *scaled(const glm::vec3 &scale);

	/* pushes a Rotation onto the decorator stack, then adds
	 * an animation to the given group
	 * Returns a pointer to the base of the stack */
	Camera *animateRotation(AnimationGroup *ag, TimeFunction<glm::vec3> *axis, TimeFunction<float> *angle);


};

class CameraDecorator : public Camera {
protected:
	Camera *next;
	bool isTos;
public:
	CameraDecorator() :
		next(NULL),
		isTos(true)
	{}

	/* pushes the given Decorator onto the decorator stack 
	 * Returns a pointer to the base of the stack */
	virtual Camera *pushDecorator(CameraDecorator *d);
	
	/* stores the current top of the decorator stack in the bucket */
	virtual Camera *store(Camera *&bucket);

	inline void setNext(Camera *next) {
		this->next = next;
	}
	
	virtual ~CameraDecorator();

	virtual void moveForward(float offset);
	virtual void moveRight(float offset);
	virtual void moveUp(float offset);

};
