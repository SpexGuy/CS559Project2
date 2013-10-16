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


View::View(Camera *c, Model *m, ViewOverlay *o) {
	camera = c;
	model = m;
	overlay = o;
}

void View::render() {
	this->setupCamera();
	mat4 context = mat4(1.0f);
	mat4 view = camera->generateViewMatrix();
	Graphics::inst()->setView(view);
	model->draw(context);

	//disable lighting and depth test for overlay
	GLboolean lit = glIsEnabled(GL_LIGHTING);
	if (lit) glDisable(GL_LIGHTING);
	GLboolean dt = glIsEnabled(GL_DEPTH_TEST);
	if (dt) glDisable(GL_DEPTH_TEST);

	overlay->draw();

	//reset depth test and lighting
	if (dt) glEnable(GL_DEPTH_TEST);
	if (lit) glEnable(GL_LIGHTING);
}

void View::setupCamera() {
	Graphics::inst()->setProjection(camera->generateProjectionMatrix());
	Graphics::inst()->setView(camera->generateViewMatrix());
}

Camera *View::getCamera() {
	return camera;
}

Model *View::getModel() {
	return model;
}


ViewOverlay::ViewOverlay() {

}

mat4 ViewOverlay::draw() const {
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
