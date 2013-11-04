#pragma once
#include "P2Cameras.h"
#include "Camera.h"
#include "Model.h"
#include "View.h"

/**
 * A Scene holds a list of cameras, along with drawables, and an overlay. 
 * The idea is that the scene has a defined set of drawables that can be
 * viewed from many different points. 
 */
class Scene
{
public:
	/**
	 * Creates a Scene with a list of cameras, a model, and an overlay.
	 */
	Scene(std::vector<Camera*> cameras, Model* model, ViewOverlay* overlay);

	/**
	 * Cycles through the cameras and switches the active camera.
	 */
	void changeCamera();

	/**
	 * Returns the model of the Scene.
	 */
	Model* getModel();

	/**
	 * Returns the overlay of the Scene.
	 */
	ViewOverlay* getOverLay();

	/**
	 * Returns the active camera of the Scene.
	 */
	Camera* getCamera();

	/**
	 * Sets the overlay of the Scene.
	 * @param overlay the overlay to the Scene.
	 */
	void setOverlay(ViewOverlay* overlay);

	/**
	 * Returns if the active camera is the last in the
	 * list of cameras in the Scene.
	 */
	bool endOfCameraList();

	/**
	 * Gets the current index of the current camera
	 * in the list of cameras.
	 */
	int getCurrentCameraIndex();

	/**
	 * Resets the active camera to the first in the
	 * list.
	 */
	void resetCameraIndex();

	~Scene();

protected:
	 //The list of cameras in the Scene.
	 std::vector<Camera*> cameras;
	 //The model of cameras in the Scene.
	 Model* model;
	 //The overlay of cameras in the Scene.
	 ViewOverlay* overlay;
	 //The index of the active camera.
	 int selectCam;
};