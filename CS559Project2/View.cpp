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

void View::render(const Graphics & g) {
	this->setProjection();
	mat4 context = mat4(1.0f);
	mat4 view = camera->generateViewMatrix();
	model->updateLighting(view, context);
	model->draw(view, context, g);

	//disable lighting and depth test for overlay
	GLboolean lit = glIsEnabled(GL_LIGHTING);
	if (lit) glDisable(GL_LIGHTING);
	GLboolean dt = glIsEnabled(GL_DEPTH_TEST);
	if (dt) glDisable(GL_DEPTH_TEST);

	overlay->draw(g);

	//reset depth test and lighting
	if (dt) glEnable(GL_DEPTH_TEST);
	if (lit) glEnable(GL_LIGHTING);
}

void View::reshape(int x, int y) {
	overlay->reshape(x, y);
	camera->reshape(x, y);
}

void View::setProjection() {
	mat4 prj = camera->generateProjectionMatrix();
	Graphics::inst.setProjection(prj);
}

Camera *View::getCamera() {
	return camera;
}

Model *View::getModel() {
	return model;
}



ViewOverlay::ViewOverlay() {
	size = ivec2(1, 1);
}

void ViewOverlay::reshape(int x, int y) {
	if (y > 0) {
		size = ivec2(x, y);
	}
}

mat4 ViewOverlay::setProjection() const {
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(
		value_ptr(
			ortho(0.0f, float(size.x),
				  0.0f, float(size.y),
				  0.0f, 1.0f)));

	return lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f));
}



