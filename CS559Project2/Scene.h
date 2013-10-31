#pragma once
#include "Camera.h"
#include "Model.h"
#include "View.h"

class Scene
{
public:
	Scene(std::vector<DynamicProjectionCamera*> cameras, Model* model, ViewOverlay* overlay);
	void changeCamera();

	Model* getModel();
	ViewOverlay* getOverLay();
	DynamicProjectionCamera* getCamera();
	bool endOfCameraList();
	int getCurrentCameraIndex();
	void resetCameraIndex();

	~Scene();

protected:
	 std::vector<DynamicProjectionCamera*> cameras;
	 Model* model;
	 ViewOverlay* overlay;
	 int selectCam;
};