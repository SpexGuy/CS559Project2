#pragma once
#include <list>
#include "Graphics.h"
#include <glm/glm.hpp>

/**
 * A class which can be drawn
 */
class Drawable {
public:
	//context is passed by value to avoid changes propagating up the call stack
	virtual void draw(glm::mat4 model) = 0;
};

/**
 * A Drawable which aggregates Drawables
 */
class DrawableGroup : public Drawable {
public:
	DrawableGroup();

	/* draws all drawables in the list */
	virtual void draw(glm::mat4 model);

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