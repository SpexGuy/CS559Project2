#include <iostream>
#include "View.h"
#include "Camera.h"
#include "Model.h"
#include "Graphics.h"
#include <glm/glm.hpp>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;


View::View(Projection *p, Camera *c, Model *m, ViewOverlay *o) {
	proj = p;
	camera = c;
	model = m;
	overlay = o;
}

void View::render() {
	this->setupCamera();
	mat4 context = mat4(1.0f);

	glEnable(GL_DEPTH_TEST);
	model->draw(context);

	glDisable(GL_DEPTH_TEST);
	overlay->draw();
}

void View::setupCamera() {
	Graphics::inst()->setProjection(proj->generateProjectionMatrix());
	Graphics::inst()->setView(camera->generateViewMatrix());
}



ViewOverlay::ViewOverlay() {

}

mat4 ViewOverlay::draw() {
	return mat4(1.0f);
}

void ViewOverlay::setupCamera() const {
	Graphics::inst()->setProjection(
			ortho(0.0f, float(Graphics::inst()->getSize().x),
				  0.0f, float(Graphics::inst()->getSize().y),
				  0.0f, 1.0f));
	Graphics::inst()->setView(
			lookAt(vec3(0.0f, 0.0f, 0.0f),
				   vec3(0.0f, 0.0f, -1.0f),
				   vec3(0.0f, 1.0f, 0.0f)));
}
