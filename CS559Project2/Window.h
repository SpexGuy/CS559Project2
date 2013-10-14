#pragma once
#include "View.h"
#include "Graphics.h"
#include <glm/glm.hpp>

/**
 * A Window is responsible for arranging and rendering Views.
 */
class Window {
protected:
	int windowHandle; //the GL window index
	bool windowClosed;
	Graphics graphics; //there must be a different Graphics for each window because we are using CallLists
	glm::ivec2 size;
public:
	/* DOES NOT initialize() the window !!!
	 * This should be done after GL is initialized.*/
	Window();

	/* tells GL to focus on this window */
	void activate();
	
	/* do any one-time initialization tasks.
	 * This should be called before any other functions.
	 * The window is left active after this function.*/
	virtual bool initialize(char *name);
	
	/* renders the window
	 * The window is left active after this function. */
	virtual void render() = 0;
	
	/* called when the window is reshaped
	 * DOES NOT change the focused window. */
	virtual void reshape(int x, int y);
	
	/* called when the window is closed
	 * DOES NOT change the focused window. */
	virtual void onClose();
	
	/* activates the window and triggers glutPostRedisplay() */
	virtual void update();
};

/**
 * A window which contains a single View that fills the screen
 */
class SingleViewportWindow : public Window {
private:
	SingleViewportWindow();
protected:
	View *view;
public:
	SingleViewportWindow(View *view);
	virtual void reshape(int x, int y);
	virtual void render();
};

/**
 * A window which contains two views: one on the left half
 * and the other on the right half.
 */
class DoubleViewportWindow : public Window {
private:
	DoubleViewportWindow();
protected:
	View *leftView;
	View *rightView;
public:
	DoubleViewportWindow(View *leftView, View *rightView);
	virtual void reshape(int x, int y);
	virtual void render();
};