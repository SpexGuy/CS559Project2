#include "Transformable.h"
#include "Graphics.h"
#include <glm/glm.hpp>

using namespace glm;

void TransformableMixin::transform(mat4 & context) const {
	TranslatableMixin::transform(context);
	RotatableMixin::transform(context);
	ScaleableMixin::transform(context);
}

void TransformableGroup::draw(mat4 model) const {
	transform(model);
	DrawableGroup::draw(model);
}