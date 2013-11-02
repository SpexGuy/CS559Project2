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
	virtual Drawable *rotated(const glm::vec3 &axis,
										const float &angle);
	
	/* pushes a Translation onto the decorator stack
	 * Returns a pointer to the base of the stack */
	virtual Drawable *translated(const glm::vec3 &position);
	
	/* pushes a Scale onto the decorator stack
	 * Returns a pointer to the base of the stack */
	virtual Drawable *scaled(const glm::vec3 &scale);
	
	/* pushes a BillboardTransform onto the decorator stack
	 * Returns a pointer to the base of the stack */
	virtual Drawable *billboard(const glm::vec3 &axis);

	/* pushes a Rotation onto the decorator stack, then adds
	 * an animation to the given group
	 * Returns a pointer to the base of the stack */
	virtual Drawable *animateRotation(
				AnimationGroup *ag,
				TimeFunction<glm::vec3> *axis,
				TimeFunction<float> *angle);
	
	/* pushes a DisableDepthTest onto the decorator stack
	 * Returns a pointer to the base of the stack */
	virtual Drawable *disableDepthTest();

	/* pushes a Color onto the decorator stack
	 * Returns a pointer to the base of the stack */
	virtual Drawable *inColor(glm::vec4 color);
	
	/* pushes a Material onto the decorator stack
	 * Returns a pointer to the base of the stack */
	virtual Drawable *inMaterial(const float &ambient, const glm::vec4 &specular, const float &shininess);
	
	/* pushes a ColorReset onto the decorator stack
	 * Returns a pointer to the base of the stack */
	virtual Drawable *resetColor();

	/* pushes a MaterialReset onto the decorator stack
	 * Returns a pointer to the base of the stack */
	virtual Drawable *resetMaterial();

	/* stores the current top of the decorator stack in the bucket */
	virtual Drawable *store(Drawable *&bucket);

	/* initializes the object with GL.
	 * Returns whether it was successful*/
	virtual bool initialize() = 0;

	/* Draws the object. */
	virtual void draw(const glm::mat4 &model) = 0;

	/* frees any GL handles still active */
	virtual void takeDown() = 0;

	virtual ~Drawable() {}
};

class DrawableDecorator : public Drawable {
private:
	DrawableDecorator();
	/* copying a DrawableDecorator breaks the memory management scheme */
	DrawableDecorator(Drawable &copy);
protected:
	Drawable *child;
	bool isTos;
	
	DrawableDecorator(Drawable *child) :
		child(child),
		isTos(true)
	{}
	
public:
	/* pushes a Rotation onto the decorator stack */
	virtual Drawable *rotated(const glm::vec3 &axis, const float &angle);
	/* pushes a Translation onto the decorator stack */
	virtual Drawable *translated(const glm::vec3 &position);
	/* pushes a Scale onto the decorator stack */
	virtual Drawable *scaled(const glm::vec3 &scale);
	/* pushes a BillboardTransform onto the decorator stack */
	virtual Drawable *billboard(const glm::vec3 &axis);
	/* pushes a Rotation onto the decorator stack, then adds an animation to the given group */
	virtual Drawable *animateRotation(AnimationGroup *ag, TimeFunction<glm::vec3> *axis, TimeFunction<float> *angle);
	/* pushes a DisableDepthTest onto the decorator stack */
	virtual Drawable *disableDepthTest();
	/* pushes a Color onto the decorator stack */
	virtual Drawable *inColor(glm::vec4 color);
	/* pushes a Material onto the decorator stack */
	virtual Drawable *inMaterial(const float &ambient, const glm::vec4 &specular, const float &shininess);
	/* pushes a ColorReset onto the decorator stack */
	virtual Drawable *resetColor();
	/* pushes a MaterialReset onto the decorator stack */
	virtual Drawable *resetMaterial();
	

	/* stores the current top of the decorator stack in the bucket */
	virtual Drawable *store(Drawable *&bucket);

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
	virtual void draw(const glm::mat4 &model);

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
	
	virtual void draw(const glm::mat4 &model);
};

class Color : public DrawableDecorator {
private:
	Color();
protected:
	glm::vec4 color;
public:
	Color(Drawable *next, const glm::vec4 &color) :
		DrawableDecorator(next),
		color(color)
	{}
	
	virtual void draw(const glm::mat4 &model);
};

class Material : public DrawableDecorator {
private:
	Material();
protected:
	float ambient, shininess;
	glm::vec4 specularColor;
public:
	Material(Drawable *next, const float &ambient, const glm::vec4 &specularColor, const float &shininess) :
		DrawableDecorator(next),
		ambient(ambient),
		shininess(shininess),
		specularColor(specularColor)
	{}
	
	virtual void draw(const glm::mat4 &model);
};

class ColorReset : public DrawableDecorator {
private:
	ColorReset();
public:
	ColorReset(Drawable *next) :
		DrawableDecorator(next)
	{}
	
	virtual void draw(const glm::mat4 &model);
};

class MaterialReset : public DrawableDecorator {
private:
	MaterialReset();
public:
	MaterialReset(Drawable *next) :
		DrawableDecorator(next)
	{}
	
	virtual void draw(const glm::mat4 &model);
};

class BillboardTransform : public DrawableDecorator {
private:
	BillboardTransform();
protected:
	glm::vec3 axis;
public:
	BillboardTransform(Drawable *next, const glm::vec3 &axis) :
		DrawableDecorator(next),
		axis(glm::normalize(axis))
	{}

	virtual void draw(const glm::mat4 &model);
};

