#define _USE_MATH_DEFINES
#include <math.h>
#include "SplineEditor.h"
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

vec2 getPoint(SplinePoint *before, SplinePoint *after, float t) {
	vec2 p0000 = before->position;
	vec2 p0001 = before->getAfterPoint();
	vec2 p0002 = after->getBeforePoint();
	vec2 p0003 = after->position;
	vec2 p000 = mix(p0000, p0001, t);
	vec2 p001 = mix(p0001, p0002, t);
	vec2 p002 = mix(p0002, p0003, t);
	vec2 p00 = mix(p000, p001, t);
	vec2 p01 = mix(p001, p002, t);
	return mix(p00, p01, t);
}


SplineEditor::SplineEditor(int numPoints) : points(numPoints) {
	assert(numPoints >= 2);
	currentIndex = 0;
	points[0] = new SplinePoint(vec2(0, 0), 90, 0.1f);
	for (int c = 1; c < numPoints-1; c++) {
		points[c] = new SplinePoint(vec2(float(c)/(numPoints-1), 1.0f/6), 0, 0.1f);
	}
	points[numPoints - 1] = new SplinePoint(vec2(1, 0), -90, 0.1f);
}

bool SplineEditor::initialize() {
	return true;
}

void SplineEditor::draw(const mat4 &model) {
	for (unsigned int c = 0; c < points.size(); c++) {
		drawSplinePoint(model, c);
	}
	drawSpline(model);
}

void SplineEditor::takeDown() {
}

void SplineEditor::drawSplinePoint(const mat4 &base, int index) {
	Graphics *g = Graphics::inst();
	SplinePoint *p = points[index];
	if (index == currentIndex)
		g->setColor(YELLOW);
	else
		g->setColor(RED);
	mat4 pos = translate(base, vec3(p->position.x, p->position.y, 0.0f));
	g->drawCircle2D(pos, 0, 0, 0.01f);
	vec2 before = p->getBeforePoint();
	vec2 after = p->getAfterPoint();
	vec2 offset(0.005f, 0.005f);
	g->drawRect2D(base, before-offset, before+offset);
	g->drawRect2D(base, after-offset, after+offset);
	g->drawLine2D(base, before, after);
}

void SplineEditor::drawSpline(const mat4 &base) {
	Graphics::inst()->setColor(GREEN);
	int numPoints = 20;
	vec2 lastPoint;
	vec2 newPoint;
	for (uint p = 0; p < points.size()-1; p++) {
		lastPoint = points[p]->position;
		for (int c = 1; c < numPoints; c++) {
			newPoint = getPoint(points[p], points[p+1], float(c)/numPoints);
			Graphics::inst()->drawLine2D(base, lastPoint, newPoint);
			lastPoint = newPoint;
		}
		Graphics::inst()->drawLine2D(base, lastPoint, points[p+1]->position);
	}
}

void SplineEditor::setupCamera() const {
	//ivec2 size = Graphics::inst()->getSize();
	//Graphics::inst()->setProjection(
	//	ortho(0.0f, 1.0f, 0.0f, float(size.y)/size.x, 0.0f, 1.0f));
	//Graphics::inst()->setView(
	//	lookAt(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
}

SplinePoint *SplineEditor::currentPoint() {
	return points[currentIndex];
}

bool SplineEditor::isEdgeSelected() {
	return currentIndex == 0 || currentIndex == points.size()-1;
}

void SplineEditor::next() {
	currentIndex++;
	currentIndex %= points.size();
}

void SplineEditor::moveVertical(float offset) {
	if (isEdgeSelected())
		return;
	SplinePoint *p = currentPoint();
	p->position.y += offset;
	if (p->position.y > 1.0f/4)
		p->position.y = 1.0f/4;
	else if (p->position.y < 0)
		p->position.y = 0;
}
void SplineEditor::moveHorizontal(float offset) {
	if (isEdgeSelected())
		return;
	SplinePoint *p = currentPoint();
	p->position.x += offset;
	if (p->position.x > 1)
		p->position.x = 1;
	else if (p->position.x < 0)
		p->position.x = 0;
}
void SplineEditor::addSize(float offset) {
	SplinePoint *p = currentPoint();
	p->distance += offset;
	if (p->distance > 0.5)
		p->distance = 0.5;
	else if (p->distance < 0)
		p->distance = 0;
}
void SplineEditor::addAngle(float offset) {
	if (isEdgeSelected())
		return;
	SplinePoint *p = currentPoint();
	p->angle += offset;
	p->angle = fmod(p->angle, 360.0f);
}

vector<vec2> SplineEditor::getSpline(int resolution) {
	vector<vec2> spline;
	for (uint c = 0; c < points.size()-1; c++) {
		for (int d = 0; d < resolution; d++) {
			spline.push_back(getPoint(points[c], points[c+1], float(d)/resolution));
		}
	}
	spline.push_back(points[points.size()-1]->position);
	return spline;
}



SplineEditor::~SplineEditor() {
	while(points.size() > 0) {
		delete points[points.size()-1];
		points.pop_back();
	}
}



vec2 SplinePoint::getBeforePoint() {
	return position - vec2(distance*cos(angle * M_PI/180.0f), distance*sin(angle * M_PI/180.0f));
}

vec2 SplinePoint::getAfterPoint() {
	return position + vec2(distance*cos(angle * M_PI/180.0f), distance*sin(angle * M_PI/180.0f));
}
