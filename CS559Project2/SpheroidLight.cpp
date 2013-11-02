#include "SpheroidLight.h"

using namespace glm;

bool SpheroidLight::initialize() {
	return true;
}

void SpheroidLight::draw(const mat4 &model) {
	mat4 m = model;
	transform(m);
	vec3 pos = vec3(m * vec4(0.0f, 0.0f, 0.0f, 1.0f));
	Graphics::inst()->setLight(pos);
}

void SpheroidLight::takeDown() {

}

