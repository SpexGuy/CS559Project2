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
