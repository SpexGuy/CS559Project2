#pragma once
#include "P2Cameras.h"
#include "Camera.h"
#include "Model.h"
#include "View.h"

class Scene
{
public:
	Scene(std::vector<MoveableCamera*> cameras, Model* model, ViewOverlay* overlay);
	void changeCamera();

	Model* getModel();
	ViewOverlay* getOverLay();
	MoveableCamera* getCamera();
	bool endOfCameraList();
	int getCurrentCameraIndex();
	void resetCameraIndex();

	~Scene();

protected:
	 std::vector<MoveableCamera*> cameras;
	 Model* model;
	 ViewOverlay* overlay;
	 int selectCam;
};