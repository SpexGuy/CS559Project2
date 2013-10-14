#pragma once
#include <glm/glm.hpp>
#include "Transformer.h"

/**
 * An interface for something that can be rotated
 */
class Rotatable : public Transformer {
public:
	virtual void setRotation(glm::vec3 & axis, float angle) = 0;
};

/**
 * A mixin class to facilitate implementation of the Rotatable interface
 */
class RotatableMixin : public Rotatable {
protected:
	float angle;
	glm::vec3 axis;
	virtual void transform(glm::mat4 & context) const;

public:
	/* initializes angle to 0 and axis to (0, 1, 0) */
	RotatableMixin();
	virtual void setRotation(glm::vec3 & axis, float angle);
};

/**
 * A mixin which gives data members and modifying functions
 * to represent a spherical coordinate system
 */
class SphericalCoordinateMixin {
protected:
	float angle;
	float axisAngle;
public:
	/* initializes angle to 0, and axisAngle to 90 */
	SphericalCoordinateMixin();

	void setAngle(float angle);

	void addAngle(float difference);
	
	/* axisAngle will be put into the range [1, 179],
	 * with 1 being close to parallel to the axis
	  * and 90 being perpendicular */
	void setAxisAngle(float axisAngle);
	
	void addAxisAngle(float difference);
};

/**
 * A Transformer mixin which implements rotation using
 * a spherical coordinate system
 */
class SphericalRotationMixin : public Transformer, public SphericalCoordinateMixin {
protected:
	virtual void transform(glm::mat4 & context) const;

};