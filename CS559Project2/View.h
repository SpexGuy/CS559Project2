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
	virtual void render(const Graphics & g);
	virtual void reshape(int x, int y);
	Camera *getCamera();
	Model *getModel();
private:
	View(); //force use of constructor with arguments
	glm::mat4 setProjection();
protected:
	ViewOverlay *overlay;
	Camera *camera;
	Model *model;
};

/** An abstract representation of a 2D overlay to be drawn on a view */
class ViewOverlay {
public:
	ViewOverlay();
	virtual void reshape(int x, int y);
	/* renders the overlay. Returns the rendering context for the overlay. */
	virtual glm::mat4 draw(const Graphics &g) const;
protected:
	glm::ivec2 size;
	/* sets up an orthographic projection matrix with the bottom left corner
	 * at (0, 0) and the top right at (size.x, size.y), the near plane at
	 * 0 and the far plane at 1. 
	 * Returns the base rendering context at pixel (0, 0)*/
	virtual glm::mat4 setProjection() const;
};
