#include "Scaleable.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

ScaleableMixin::ScaleableMixin() {
	this->scaleVec = vec3(1.0f, 1.0f, 1.0f);
}

vec3 ScaleableMixin::getScale() const {
	return this->scaleVec;
}

void ScaleableMixin::scale(vec3 newScale) {
	this->scaleVec = newScale;
}

void ScaleableMixin::transform(glm::mat4 & context) const {
	context = glm::scale(context, this->scaleVec);
}



void Scale::draw(mat4 model) {
	transform(model);
	child->draw(model);
}



mat4 CamScale::generateViewMatrix() {
	mat4 view = next->generateViewMatrix();
	view = glm::scale(view, vec3(1.0f/scaleVec.x, 1.0f/scaleVec.y, 1.0f/scaleVec.z));
	return view;
}
