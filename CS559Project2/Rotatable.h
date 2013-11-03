#pragma once
#include <glm/glm.hpp>
#include "Transformer.h"
#include "Drawable.h"
#include "Camera.h"

/**
 * An interface for something that can be rotated
 */
class Rotatable : public Transformer {
public:
	virtual void setRotation(const glm::vec3 &axis, const float &angle) = 0;
};

/**
 * A mixin class to facilitate implementation of the Rotatable interface
 */
//@Deprecated
class RotatableMixin : public Rotatable {
protected:
	float angle;
	glm::vec3 axis;
	virtual void transform(glm::mat4 & context) const;

public:
	/* initializes angle to 0 and axis to (0, 1, 0) */
	RotatableMixin();
	virtual void setRotation(const glm::vec3 &axis, const float &angle);
};

class Rotation : public RotatableMixin, public DrawableDecorator {
public:
	Rotation() :
		DrawableDecorator() {}
	virtual void draw(const glm::mat4 &model);
};

class CamRotation : public RotatableMixin, public CameraDecorator {
public:
	CamRotation() :
		CameraDecorator() {}
	virtual glm::mat4 generateViewMatrix();
};

/**
 * A mixin which gives data members and modifying functions
 * to represent a spherical coordinate system
 */
class SphericalAngleMixin {
protected:
	float angle;
	float axisAngle;
public:
	inline SphericalAngleMixin() {
		this->angle = 0.0f;
		this->axisAngle = 90.0f;
	}

	inline void setAngle(float angle) {
		this->angle = float(fmod(angle, 360.0f));
	}
	inline void addAngle(float difference) {
		setAngle(angle+difference);
	}
	
	/* axisAngle will be clamped into the range [1, 179],
	 * with 1 being close to parallel to the axis
	  * and 90 being perpendicular */
	inline void setAxisAngle(float axisAngle) {
		if (axisAngle < 1)
			axisAngle = 1.0f;
		if (axisAngle > 179)
			axisAngle = 179.0f;
		this->axisAngle = axisAngle;
	}
	inline void addAxisAngle(float difference) {
		setAxisAngle(axisAngle+difference);
	}
};

class SphericalCoordinateMixin : public SphericalAngleMixin {
protected:
	float radius;
public:
	inline SphericalCoordinateMixin() {
		this->radius = 1;
	}
	inline void setRadius(float radius) {
		if (radius <= 0)
			radius = 0.0000001f;
		this->radius = radius;
	}
	inline void addRadius(float difference) {
		setRadius(radius+difference);
	}
};

/**
 * A Transformer mixin which implements rotation using
 * a spherical coordinate system
 */
class SphericalRotationMixin : public Transformer, public SphericalAngleMixin {
protected:
	virtual void transform(glm::mat4 & context) const;

};

class SphericalPositionMixin : public Transformer, public SphericalCoordinateMixin {
protected:
	virtual void transform(glm::mat4 & context) const;
};