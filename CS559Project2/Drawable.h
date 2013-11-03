#pragma once
#include <list>
#include "Graphics.h"
#include "Function.h"
#include <glm/glm.hpp>

class AnimationGroup;
class DrawableDecorator;

/**
 * A class which can be drawn. It has support for decorators.
 */
class Drawable {
public:
	/* initializes the object with GL.
	 * Returns whether it was successful*/
	virtual bool initialize() = 0;

	/* Draws the object. */
	virtual void draw(const glm::mat4 &model) = 0;

	/* frees any GL handles still active */
	virtual void takeDown() = 0;

	virtual ~Drawable() {}

	/* pushes the given decorator onto the decorator stack
	 * Returns a pointer to the base of the stack */
	virtual Drawable *pushDecorator(DrawableDecorator *d);

	/* stores the current top of the decorator stack in the bucket */
	virtual Drawable *store(Drawable *&bucket);

	//--------- Decorator Functions -----------------
	//The following functions exist only as wrappers to pushDecorator
	//to enrich the syntax

	/* pushes a Rotation onto the decorator stack 
	 * Returns a pointer to the base of the stack */
	Drawable *rotated(const glm::vec3 &axis,
						const float &angle);
	
	/* pushes a Translation onto the decorator stack
	 * Returns a pointer to the base of the stack */
	Drawable *translated(const glm::vec3 &position);
	
	/* pushes a Scale onto the decorator stack
	 * Returns a pointer to the base of the stack */
	Drawable *scaled(const glm::vec3 &scale);
	
	/* pushes a BillboardTransform onto the decorator stack
	 * Returns a pointer to the base of the stack */
	Drawable *billboard(const glm::vec3 &axis);

	/* pushes a Rotation onto the decorator stack, then adds
	 * an animation to the given group
	 * Returns a pointer to the base of the stack */
	Drawable *animateRotation(
				AnimationGroup *ag,
				TimeFunction<glm::vec3> *axis,
				TimeFunction<float> *angle);
	
	/* pushes a DisableDepthTest onto the decorator stack
	 * Returns a pointer to the base of the stack */
	Drawable *disableDepthTest();
	
	/* pushes a Color onto the decorator stack
	 * Returns a pointer to the base of the stack */
	Drawable *inColor(glm::vec4 color);
	
	/* pushes a Material onto the decorator stack
	 * Returns a pointer to the base of the stack */
	Drawable *inMaterial(const float &ambient,
		const glm::vec4 &specular, const float &shininess);
	
	/* pushes a ColorReset onto the decorator stack
	 * Returns a pointer to the base of the stack */
	Drawable *resetColor();

	/* pushes a MaterialReset onto the decorator stack
	 * Returns a pointer to the base of the stack */
	Drawable *resetMaterial();

	/* pushes a ModelviewMode onto the decorator stack
	 * Returns a pointer to the base of the stack */
	Drawable *useMVMode(int mode);
	
};

/**
 * an abstract class which all decorators should extend
 *
 * The decorators form a stack on top of the underlying
 * Drawable. This stack is upside-down so that the order
 * in which the decorators are activated is the same as
 * the order in which they were pushed onto the stack.
 *
 * Decorators manage their own memory, and will delete
 * child pointers all the way down the stack including
 * the terminating Drawable.
 */
class DrawableDecorator : public Drawable {
private:
	/* copying a DrawableDecorator breaks the memory management scheme */
	DrawableDecorator(Drawable &copy);
protected:
	Drawable *child;
	bool isTos;
	
	DrawableDecorator() :
		child(NULL),
		isTos(true)
	{}

public:
	/* {@InheritDoc} */
	virtual Drawable *pushDecorator(DrawableDecorator *d);

	/* {@InheritDoc} */
	virtual Drawable *store(Drawable *&bucket);

	/* propagates initialization down the stack */
	virtual bool initialize();
	
	/* propagates takeDown down the stack */
	virtual void takeDown();

	inline void setChild(Drawable *child) {
		this->child = child;
	}
	inline Drawable *getChild() {
		return child;
	}

	/* recursively deletes children. */
	virtual ~DrawableDecorator();
};

/**
 * A Drawable which aggregates Drawables
 */
class DrawableGroup : public Drawable {
public:
	DrawableGroup();

	/* initializes all children */
	virtual bool initialize();

	/* draws all drawables in the list */
	virtual void draw(const glm::mat4 &model);

	/* takes down all children */
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

/**
 * Disables the depth test on the way down the decorator stack,
 * then reenables it on the way back up.
 */
class DisableDepthTest : public DrawableDecorator {
public:
	DisableDepthTest() :
		DrawableDecorator() {}
	
	virtual void draw(const glm::mat4 &model);
};

/** Sets the color on the way down the decorator stack.
 * DOES NOT RESET IT. Push a ColorReset before this to
 * reset the color. */
class Color : public DrawableDecorator {
private:
	Color();
protected:
	glm::vec4 color;
public:
	Color(const glm::vec4 &color) :
		DrawableDecorator(),
		color(color)
	{}
	
	virtual void draw(const glm::mat4 &model);
};

/** Sets the material on the way down the decorator stack.
 * DOES NOT RESET IT. Push a MaterialReset before this to
 * reset the material. */
class Material : public DrawableDecorator {
private:
	Material();
protected:
	float ambient, shininess;
	glm::vec4 specularColor;
public:
	Material(const float &ambient, const glm::vec4 &specularColor, const float &shininess) :
		DrawableDecorator(),
		ambient(ambient),
		shininess(shininess),
		specularColor(specularColor)
	{}
	
	virtual void draw(const glm::mat4 &model);
};

/** Saves the color on the way down the stack, and restores
 * it on the way back up. */
class ColorReset : public DrawableDecorator {
public:
	ColorReset() :
		DrawableDecorator()
	{}
	
	virtual void draw(const glm::mat4 &model);
};

/** Saves the material on the way down the stack, and restores
 * it on the way back up. */
class MaterialReset : public DrawableDecorator {
public:
	MaterialReset() :
		DrawableDecorator()
	{}
	
	virtual void draw(const glm::mat4 &model);
};

/** Creates a transformation which makes the object billboard
 * around the given local axis, and passes it down the stack. */
class BillboardTransform : public DrawableDecorator {
private:
	BillboardTransform();
protected:
	glm::vec3 axis;
public:
	BillboardTransform(const glm::vec3 &axis) :
		DrawableDecorator(),
		axis(glm::normalize(axis))
	{}

	virtual void draw(const glm::mat4 &model);
};

/** Changes the modelview mode on the way down the stack
 * Resets it on the way back up */
class ModelviewMode : public DrawableDecorator {
private:
	ModelviewMode();
protected:
	int mode;
public:
	ModelviewMode(const int &mode) :
		DrawableDecorator(),
		mode(mode)
	{}

	virtual void draw(const glm::mat4 &model);
};
