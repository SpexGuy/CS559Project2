#pragma once
#include <vector>
#include "View.h"

class SplinePoint;

class SplineEditor : public Drawable {
private:
	SplineEditor();
	SplinePoint *currentPoint();
	bool isEdgeSelected();
protected:
	int currentIndex;
	std::vector<SplinePoint *> points;
	void drawSplinePoint(const glm::mat4 &model, int index);
	void drawSpline(const glm::mat4 &model);
public:
	/** creates a bezier spline editor with the given number of points.
	 * two of the points, the endpoints, cannot be moved or rotated. */
	SplineEditor(int numPoints);

	/** discretizes the spline with resolution points per spline segment */
	std::vector<glm::vec2> getSpline(int resolution);

	/** advances the current splinePoint */
	void next();

	/** moves the current point upward */
	void moveVertical(float offset);

	/** moves the current point to the right */
	void moveHorizontal(float offset);

	/** adds to the distance between control points for the current point */
	void addSize(float offset);

	/** adds to the angle of the current point */
	void addAngle(float offset);

	/** {@InheritDoc} */
	virtual bool initialize();
	/** {@InheritDoc} */
	virtual void draw(const glm::mat4 &model);
	/** {@InheritDoc} */
	virtual void takeDown();

	virtual ~SplineEditor();
};

class SplinePoint {
public:
	SplinePoint(glm::vec2 position, float angle, float distance)
		: position(position), angle(angle), distance(distance) {}
	float angle;
	float distance;
	glm::vec2 position;
	
	glm::vec2 getBeforePoint();
	glm::vec2 getAfterPoint();
};