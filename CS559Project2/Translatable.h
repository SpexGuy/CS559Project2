#pragma once
#include <glm/glm.hpp>
#include "Transformer.h"
#include "Drawable.h"

/**
 * An interface representing an object which can be translated
 */
class Translatable : public Transformer {
public:
	virtual void translate(const glm::vec3 & offset) = 0;
	virtual glm::vec3 position() const = 0;
	virtual void position(glm::vec3 pos) = 0;
};

/**
 * A mixin to facilitate implementation of the Translatable interface
 */
class TranslatableMixin : public Translatable {
public:
	/* initializes the position to (0, 0, 0) */
	TranslatableMixin();
	virtual void translate(const glm::vec3 & offset);
	virtual glm::vec3 position() const;
	virtual void position(glm::vec3 pos);
protected:
	virtual void transform(glm::mat4 & context) const;
private:
	glm::vec3 pos;
};


class Translation : public TranslatableMixin, public DrawableDecorator {
private:
	Translation();
public:
	Translation(Drawable *child) :
		DrawableDecorator(child) {}
	virtual void draw(glm::mat4 model);
};
