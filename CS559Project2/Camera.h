#pragma once
#include <glm/glm.hpp>
#include "Translatable.h"
#include "Rotatable.h"

/** 
 * A camera represents a projection and a positon, and therefore
 * is responsible for generating a projection matrix and a view matrix.
 */
class Camera {
public:
	virtual glm::mat4 generateProjectionMatrix() = 0;
	virtual glm::mat4 generateViewMatrix() = 0;
};

/**
 * A Projection is responsible for generating a projection matrix.
 */
class Projection {
protected:
	float hither, yon;
public:
	Projection();
	virtual glm::mat4 generateProjectionMatrix() = 0;
	void setPlanes(float hither, float yon);
};

class OrthogonalProjection : public Projection {
private:
	OrthogonalProjection();
protected:
	float height;
public:
	OrthogonalProjection(float height);
	virtual glm::mat4 generateProjectionMatrix();
	/* sets the minimum radius of display for the orthogonal projection. */
	void setHeight(float height);
};

class PerspectiveProjection : public Projection {
private:
	PerspectiveProjection();
protected:
	float fov;
public:
	PerspectiveProjection(float fov);
	virtual glm::mat4 generateProjectionMatrix();
	/* sets the vertical fov angle.
	 * fov will be put into the range [10, 80] */
	virtual void setFov(float fov);
	void addFov(float fov);
	float getFov();
};

/**
 * A camera which uses a Projection to generate the projection matrix
 */
class DynamicProjectionCamera : public Camera {
protected:
	Projection *projection;
private:
	DynamicProjectionCamera();
public:
	DynamicProjectionCamera(Projection *proj);
	virtual glm::mat4 generateProjectionMatrix();
};

/**
 * A camera which is fixed to the surface of a sphere and points at its center
 */
class SpheroidCamera : public DynamicProjectionCamera, public SphericalCoordinateMixin {
public:
	SpheroidCamera(Projection *p);
	virtual glm::mat4 generateViewMatrix();
};