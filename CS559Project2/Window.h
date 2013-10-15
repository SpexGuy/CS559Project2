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
	glm::ivec2 size;
	
	virtual void renderViews() = 0;

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
	void render();
	
	/* called when the window is reshaped
	 * DOES NOT change the focused window. */
	virtual void reshape(int x, int y);
	
	/* called when the window is closed
	 * DOES NOT change the focused window. */
	virtual void onClose();
	
	/* activates the window and triggers glutPostRedisplay() */
	virtual void update();

	inline bool isClosed() {
		return windowClosed;
	}
};

/**
 * A window which contains a single View that fills the screen
 */
class SingleViewportWindow : public Window {
private:
	SingleViewportWindow();
protected:
	View *view;

	virtual void renderViews();

public:
	SingleViewportWindow(View *view);
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

	virtual void renderViews();

public:
	DoubleViewportWindow(View *leftView, View *rightView);
};