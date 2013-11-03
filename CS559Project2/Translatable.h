#pragma once
#include <glm/glm.hpp>
#include "Transformer.h"
#include "Drawable.h"
#include "Camera.h"

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
protected:
	virtual void transform(glm::mat4 & context) const;
	glm::vec3 pos;
public:
	/* initializes the position to (0, 0, 0) */
	TranslatableMixin();
	virtual void translate(const glm::vec3 & offset);
	virtual glm::vec3 position() const;
	virtual void position(glm::vec3 pos);
};


class Translation : public TranslatableMixin, public DrawableDecorator {
public:
	Translation() :
		DrawableDecorator() {}
	virtual void draw(const glm::mat4 &model);
};

class CamTranslation : public TranslatableMixin, public CameraDecorator {
public:
	CamTranslation() :
		CameraDecorator() {}
	virtual glm::mat4 generateViewMatrix();
};
