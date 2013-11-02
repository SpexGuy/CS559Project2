#pragma once
#include "Transformer.h"
#include "Drawable.h"
#include "Camera.h"
#include <glm/glm.hpp>

/**
 * An interface for a transformer which can be scaled
 */
class Scaleable : public Transformer {
public:
	virtual void scale(glm::vec3 scale) = 0;
	virtual glm::vec3 getScale() const = 0;
};

/**
 * A mixin to facilitate implementation of the Scaleable interface
 */
class ScaleableMixin : public Scaleable {
public:
	/* initializes the scale to (1, 1, 1) */
	ScaleableMixin();
	virtual void scale(glm::vec3 scale);
	virtual glm::vec3 getScale() const;
protected:
	glm::vec3 scaleVec;
	virtual void transform(glm::mat4 & context) const;
};

class Scale : public ScaleableMixin, public DrawableDecorator {
private:
	Scale();
public:
	Scale(Drawable *child) :
		DrawableDecorator(child) {}
	virtual void draw(const glm::mat4 &model);
};

class CamScale : public ScaleableMixin, public CameraDecorator {
private:
	CamScale();
public:
	CamScale(Camera *next) :
		CameraDecorator(next) {}
	virtual glm::mat4 generateViewMatrix();
};

