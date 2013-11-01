#pragma once
#include <glm/glm.hpp>

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

