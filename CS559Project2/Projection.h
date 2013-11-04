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

	/** generates and returns a projection matrix */
	virtual glm::mat4 generateProjectionMatrix() = 0;

	void setPlanes(const float &hither, const float &yon);
};

/** An OrthogonalProjection generates a projection matrix
 * which does not make things smaller as they get further away */
class OrthogonalProjection : public Projection {
private:
	OrthogonalProjection();
protected:
	float height;
public:
	OrthogonalProjection(const float &height);
	virtual glm::mat4 generateProjectionMatrix();
	/* sets the minimum radius of display for the orthogonal projection. */
	void setHeight(const float &height);
};

/** A PerspectiveProjection generates a perspective
 * projection matrix which makes objects appear to shrink as they get
 * further away. */
class PerspectiveProjection : public Projection {
private:
	PerspectiveProjection();
protected:
	float fov;
public:
	PerspectiveProjection(const float &fov);
	virtual glm::mat4 generateProjectionMatrix();
	
	/* sets the vertical fov angle.
	 * fov will be clamped into the range [10, 80] */
	void setFov(const float &fov);

	inline void addFov(const float &fov) {
		setFov(this->fov += fov);
	}
	inline float getFov() const {
		return fov;
	}
};

