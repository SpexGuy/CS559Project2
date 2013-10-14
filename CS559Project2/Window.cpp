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
	this->graphics = Graphics();
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
	return graphics.initialize();
}

void Window::onClose() {
	windowClosed = true;
	glutLeaveMainLoop();
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



SingleViewportWindow::SingleViewportWindow(View *view) {
	this->view = view;
}

void SingleViewportWindow::render() {
	activate();
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, size.x, size.y);
	view->render(graphics);
	glutSwapBuffers();
}

void SingleViewportWindow::reshape(int w, int h) {
	if (windowClosed == false) {
		view->reshape(w, h);
	}
	Window::reshape(w, h);
}



DoubleViewportWindow::DoubleViewportWindow(View *leftView, View *rightView) {
	this->leftView = leftView;
	this->rightView = rightView;
}

void DoubleViewportWindow::render() {
	activate();
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	int centerX = size.x / 2;
	glViewport(0, 0, centerX, size.y);
	leftView->render(graphics);
	glViewport(centerX, 0, centerX, size.y);
	rightView->render(graphics);
	glutSwapBuffers();
}

void DoubleViewportWindow::reshape(int w, int h) {
	if (windowClosed == false) {
		leftView->reshape(w/2, h);
		rightView->reshape(w/2, h);
	}
	Window::reshape(w, h);
}
