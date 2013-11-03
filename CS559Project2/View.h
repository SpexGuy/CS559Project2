#pragma once
#include <vector>
#include "Camera.h"
#include "Projection.h"
#include "Model.h"
#include "Graphics.h"

class ViewOverlay;

/**
 * The view class encapsulates the responsibility of using a Camera
 * to render a Model.
 */
class View {
private:
	View(); //force use of constructor with arguments
	void setupCamera();
protected:
	ViewOverlay *overlay;
	Projection *proj;
	Camera *camera;
	Model *model;
public:
	View(Projection *p, Camera *camera, Model *model, ViewOverlay *o);
	
	virtual void render();
	
	inline Camera *getCamera() {
		return camera;
	}
	inline Model *getModel() {
		return model;
	}
	inline void setOverlay(ViewOverlay *o) {
		this->overlay = o;
	}
	inline void setCamera(Camera *c) {
		this->camera = c;
	}
	inline void setModel(Model *m) {
		this->model = m;
	}
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

class HudOverlay : public ViewOverlay{
public:
	virtual glm::mat4 draw();
	HudOverlay(std::vector<char*> text);

protected:
	std::vector<char*> text;
};
