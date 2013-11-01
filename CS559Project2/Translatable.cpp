#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Translatable.h"

using namespace glm;

TranslatableMixin::TranslatableMixin() {
	pos = vec3(0.0f);
}

void TranslatableMixin::transform(glm::mat4 & context) const {
	context = glm::translate(context, pos);
}

void TranslatableMixin::translate(const vec3 & offset) {
	pos += offset;
}

void TranslatableMixin::position(vec3 pos) {
	this->pos = pos;
}

vec3 TranslatableMixin::position() const {
	return pos;
}



void Translation::draw(mat4 model) {
	transform(model);
	child->draw(model);
}



mat4 CamTranslation::generateViewMatrix() {
	mat4 view = next->generateViewMatrix();
	transform(view);
	return view;
}
