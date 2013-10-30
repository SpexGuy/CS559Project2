#pragma once
#include <list>
#include "Graphics.h"
#include "Function.h"
#include <glm/glm.hpp>

class AnimationGroup;
class DrawableDecorator;

/**
 * A class which can be drawn
 */
class Drawable {
public:
	/* pushes a Rotation onto the decorator stack 
	 * Returns a pointer to the base of the stack */
	virtual DrawableDecorator *rotated(const glm::vec3 &axis,
										const float &angle);
	
	/* pushes a Translation onto the decorator stack
	 * Returns a pointer to the base of the stack */
	virtual DrawableDecorator *translated(const glm::vec3 &position);
	
	/* pushes a Scale onto the decorator stack
	 * Returns a pointer to the base of the stack */
	virtual DrawableDecorator *scaled(const glm::vec3 &scale);
	
	/* pushes a Rotation onto the decorator stack, then adds
	 * an animation to the given group
	 * Returns a pointer to the base of the stack */
	virtual DrawableDecorator *animateRotation(
				AnimationGroup *ag,
				TimeFunction<glm::vec3> *axis,
				TimeFunction<float> *angle);
	
	/* pushes a DisableDepthTest onto the decorator stack
	 * Returns a pointer to the base of the stack */
	virtual DrawableDecorator *disableDepthTest();

	/* initializes the object with GL.
	 * Returns whether it was successful*/
	virtual bool initialize() = 0;

	/* Draws the object.  Context is passed by value to avoid
	 * changes propagating up the call stack */
	virtual void draw(glm::mat4 model) = 0;

	/* frees any GL handles still active */
	virtual void takeDown() = 0;

	virtual ~Drawable() {}
};

class DrawableDecorator : public Drawable {
private:
	DrawableDecorator();
	/* copying a DrawableDecorator breaks the memory management scheme */
	DrawableDecorator(DrawableDecorator &copy);
protected:
	Drawable *child;
	bool isTos;
	
	DrawableDecorator(Drawable *child) :
		child(child),
		isTos(true)
	{}
	
public:
	/* pushes a Rotation onto the decorator stack */
	virtual DrawableDecorator *rotated(const glm::vec3 &axis, const float &angle);
	/* pushes a Translation onto the decorator stack */
	virtual DrawableDecorator *translated(const glm::vec3 &position);
	/* pushes a Scale onto the decorator stack */
	virtual DrawableDecorator *scaled(const glm::vec3 &scale);
	/* pushes a Rotation onto the decorator stack, then adds an animation to the given group */
	virtual DrawableDecorator *animateRotation(AnimationGroup *ag, TimeFunction<glm::vec3> *axis, TimeFunction<float> *angle);
	/* pushes a DisableDepthTest onto the decorator stack */
	virtual DrawableDecorator *disableDepthTest();

	/* stores the current top of the decorator stack in the bucket */
	virtual DrawableDecorator *store(DrawableDecorator **bucket);

	virtual bool initialize();
	
	virtual void takeDown();

	inline Drawable *getChild() {
		return child;
	}

	virtual ~DrawableDecorator();
};

/**
 * A Drawable which aggregates Drawables
 */
class DrawableGroup : public Drawable {
public:
	DrawableGroup();

	virtual bool initialize();

	/* draws all drawables in the list */
	virtual void draw(glm::mat4 model);

	virtual void takeDown();

	/* adds a drawable to the front of the list */
	void addLight(Drawable *light);

	/* adds a drawable to the end of the list */
	void addElement(Drawable *drawable);

	/* clears the list */
	void clearElements();

	/* returns a pointer to the list */
	std::list<Drawable*> *getElements();

private:
	std::list<Drawable*> elements;
};

class DisableDepthTest : public DrawableDecorator {
public:
	DisableDepthTest(Drawable *child) :
		DrawableDecorator(child) {}
	
	virtual void draw(glm::mat4 model);
};

