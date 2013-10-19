#pragma once
#include "Camera.h"
#include "Model.h"
#include "Graphics.h"

class ViewOverlay;

/**
 * The view class encapsulates the responsibility of using a Camera
 * to render a Model.
 */
class View {
public:
	View(Camera *camera, Model *model, ViewOverlay *o);
	virtual void render();
	Camera *getCamera();
	Model *getModel();
	inline void setOverlay(ViewOverlay *o) {
		this->overlay = o;
	}
	inline void setCamera(Camera *c) {
		this->camera = c;
	}
private:
	View(); //force use of constructor with arguments
	void setupCamera();
protected:
	ViewOverlay *overlay;
	Camera *camera;
	Model *model;
};

/** An abstract representation of a 2D overlay to be drawn on a view */
class ViewOverlay {
public:
	ViewOverlay();
	/* renders the overlay. Returns the rendering context for the overlay. */
	virtual glm::mat4 draw();
protected:
	/* sets up an orthographic projection matrix with the bottom left corner
	 * at (0, 0) and the top right at (size.x, size.y), the near plane at
	 * 0 and the far plane at 1. 
	 * Sets the view to the base rendering context at (0, 0) */
	virtual void setupCamera() const;
};
