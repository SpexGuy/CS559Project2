#pragma once
#include <vector>
#include "View.h"

class SplinePoint;

class SplineEditor : public Drawable {
private:
	SplineEditor();
	void drawSplinePoint(const glm::mat4 &model, int index);
	void drawSpline(const glm::mat4 &model);
	SplinePoint *currentPoint();
	bool isEdgeSelected();
protected:
	int currentIndex;
	std::vector<SplinePoint *> points;
	virtual void setupCamera() const;
public:
	SplineEditor(int numPoints);
	std::vector<glm::vec2> getSpline(int resolution);
	void next();
	void moveVertical(float offset);
	void moveHorizontal(float offset);
	void addSize(float offset);
	void addAngle(float offset);
	virtual bool initialize();
	virtual void draw(const glm::mat4 &model);
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