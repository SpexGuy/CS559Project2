#pragma once
#include <list>
#include "Graphics.h"
#include <glm/glm.hpp>

/**
 * A class which can be drawn
 */
class Drawable {
public:
	//called before anything is drawn to ensure that all objects are lighted
	virtual void updateLighting(const glm::mat4 & view, glm::mat4 context) const;
	//context is passed by value to avoid changes propagating up the call stack
	virtual void draw(const glm::mat4 & view, glm::mat4 context, const Graphics & g) const = 0;
};

/**
 * A Drawable which aggregates Drawables
 */
class DrawableGroup : public Drawable {
public:
	DrawableGroup();
	virtual void updateLighting(const glm::mat4 & view, glm::mat4 context) const;
	virtual void draw(const glm::mat4 & view, glm::mat4 context, const Graphics & g) const;
	void addElement(Drawable *drawable);
	std::list<Drawable*> *getElements();
private:
	std::list<Drawable*> elements;
};