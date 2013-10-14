#include "Transformable.h"
#include "Graphics.h"
#include <glm/glm.hpp>

using namespace glm;

void TransformableMixin::transform(mat4 & context) const {
	TranslatableMixin::transform(context);
	RotatableMixin::transform(context);
	ScaleableMixin::transform(context);
}

void TransformableGroup::draw(const mat4 & view, mat4 context, const Graphics & g) const {
	transform(context);
	DrawableGroup::draw(view, context, g);
}