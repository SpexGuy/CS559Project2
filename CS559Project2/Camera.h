#pragma once
#include <glm/glm.hpp>

/** 
 * A camera represents a projection and a positon, and therefore
 * is responsible for generating a projection matrix and a view matrix.
 */
class Camera {
public:
	virtual glm::mat4 generateViewMatrix() = 0;
	virtual ~Camera() {}
};

class CameraDecorator : public Camera {
private:
	CameraDecorator();
protected:
	Camera *next;
public:
	CameraDecorator(Camera *next) :
		next(next) {}

	virtual ~CameraDecorator();
};
