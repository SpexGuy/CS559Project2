#include "SpheroidLight.h"

using namespace glm;

bool SpheroidLight::initialize() {
	return true;
}

void SpheroidLight::draw(mat4 model) {
	transform(model);
	vec3 pos = vec3(model * vec4(0.0f, 0.0f, 0.0f, 1.0f));
	Graphics::inst()->setLight(pos);
}

void SpheroidLight::takeDown() {

}

