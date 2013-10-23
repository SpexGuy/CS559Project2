#include <iostream>
#include "Window.h"
#include "Graphics.h"
#include <GL/freeglut.h>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

Window::Window() {
	this->size = ivec2(1, 1);
	this->windowHandle = -1;
	this->windowClosed = true;
}

void Window::activate() {
	if (windowClosed) return;
	glutSetWindow(this->windowHandle);
}

bool Window::initialize(char *name) {
	this->windowHandle = glutCreateWindow(name);
	if (windowHandle <= 0) {
		return false;
	}
	this->windowClosed = false;
	return true;
}

void Window::onClose() {
	windowClosed = true;
}

void Window::update() {
	activate();
	if (windowClosed == false) {
		glutPostRedisplay();
	}
}

void Window::reshape(int x, int y) {
	if (windowClosed == false && y > 0) {
		size = ivec2(x, y);
		update();
	}
}

void Window::render() {
	activate();
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	renderViews();
	glutSwapBuffers();
}

void Window::toggleFullscreen() {
	activate();
	glutFullScreenToggle();
}

void Window::enterFullscreen() {
	activate();
	glutFullScreen();
}

void Window::exitFullscreen() {
	activate();
	glutLeaveFullScreen();
}



SingleViewportWindow::SingleViewportWindow(View *view) {
	this->view = view;
}

void SingleViewportWindow::renderViews() {
	Graphics::inst()->viewport(0, 0, size.x, size.y);
	view->render();
}


DoubleViewportWindow::DoubleViewportWindow(View *leftView, View *rightView) {
	this->leftView = leftView;
	this->rightView = rightView;
}

void DoubleViewportWindow::renderViews() {
	int centerX = size.x / 2;
	Graphics::inst()->viewport(0, 0, centerX, size.y);
	leftView->render();
	Graphics::inst()->viewport(centerX, 0, centerX, size.y);
	rightView->render();
}
