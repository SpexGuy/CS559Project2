#include "Scene.h"

using namespace std;
using namespace glm;

Scene::Scene(vector<DynamicProjectionCamera*> cameras, Model *model, ViewOverlay *overlay)
{
	this->cameras = cameras;
	this->model = model;
	this ->overlay = overlay;
	this->selectCam = 0;
}

void Scene::changeCamera()
{
	if( selectCam == cameras.size()-1)
		selectCam = 0;
	else
		selectCam++;

	DynamicProjectionCamera *c = cameras[selectCam];
}

Model* Scene::getModel()
{
	return model;
}

ViewOverlay* Scene::getOverLay()
{
	return overlay;
}

DynamicProjectionCamera* Scene::getCamera()
{
	return cameras[selectCam];
}

bool Scene::endOfCameraList()
{
	return(selectCam == cameras.size()-1);
}

int Scene::getCurrentCameraIndex()
{
	return selectCam;
}

void Scene::resetCameraIndex()
{
	selectCam =0;
}