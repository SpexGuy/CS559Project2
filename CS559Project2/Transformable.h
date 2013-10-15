#pragma once
#include "Rotatable.h"
#include "Scaleable.h"
#include "Translatable.h"
#include "Drawable.h"
#include "Graphics.h"
#include <glm/glm.hpp>

/**
 * Combines the abilities of Rotatable, Scaleable, and Translatable into a single interface
 */
class Transformable : public Rotatable, public Scaleable, public Translatable {

};

/**
 * A mixin for quick implementation of the Transformable interface.
 */
class TransformableMixin : public RotatableMixin, public ScaleableMixin, public TranslatableMixin {
public:
	/* Translates, then rotates, then scales */
	virtual void transform(glm::mat4 & context) const;
};

/**
 * A DrawableGroup which can be transformed as a whole
 */
class TransformableGroup : public DrawableGroup, public TransformableMixin {
public:
	//@Override
	virtual void draw(glm::mat4 model) const;

};