#pragma once
#include <vector>
#include "View.h"

class SplinePoint;

class SplineEditorOverlay : public ViewOverlay {
private:
	SplineEditorOverlay();
	void drawSplinePoint(const glm::mat4 &base, int index);
	void drawSpline(const glm::mat4 &base);
	SplinePoint *currentPoint();
	bool isEdgeSelected();
protected:
	int currentIndex;
	std::vector<SplinePoint *> points;
	virtual void setupCamera() const;
public:
	SplineEditorOverlay(int numPoints);
	std::vector<glm::vec2> getSpline(int resolution);
	void next();
	void moveVertical(float offset);
	void moveHorizontal(float offset);
	void addSize(float offset);
	void addAngle(float offset);
	virtual glm::mat4 draw();
	~SplineEditorOverlay();
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