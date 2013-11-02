#include <vector>
#include <glm\glm.hpp>
#include "Rocket.h"
#include "Mesh.h"

#define HEAD_HEIGHT 2.0f
#define HEAD_RADIUS 1.0f


using namespace glm;
using namespace std;

Rocket::Rocket()
{
	 this->stacks = 32;
	 this->slices = 32;
}

bool Rocket::initialize()
{
	
	float headToLeg = HEAD_RADIUS*2.0f;
	float topOfLeg = -HEAD_HEIGHT/1.625f;
	float cylinderHeight = HEAD_HEIGHT/1.25f;
	float smallRadius = HEAD_RADIUS/4.0f;
	float strutRadiusInLeg = smallRadius/2.0f;
	float strutRadiusInHead = smallRadius*1.25f;

//tanslate rotate scale in that order

	//Draws the head
	head = Mesh::newSphere(stacks, slices, HEAD_RADIUS, true)
				->scaled(vec3(1.0f, HEAD_HEIGHT, 1.0f));
	if (!head->initialize())
		return false;

	//top sphere
	Drawable* tmp = Mesh::newSphere(stacks, slices, smallRadius, true)
				->translated(vec3(headToLeg, topOfLeg, 0.0f));
	if (!tmp->initialize())
		return false;
	addElement(tmp);

	//mid cylinder
	tmp = Mesh::newCylinder(stacks, slices, smallRadius, smallRadius, true)
				->translated(vec3(0.0f, topOfLeg, headToLeg))
				->rotated(vec3(0.0f,0.0f,1.0f), 180.0f)
				->scaled(vec3(1.0f,cylinderHeight, 1.0f));
	if (!tmp->initialize())
		return false;
	addElement(tmp);

	//bottom sphere
	tmp = Mesh::newSphere(stacks, slices, smallRadius,true)
				->translated(vec3(headToLeg, topOfLeg-cylinderHeight, 0.0f))
				->scaled(vec3(1.0f,cylinderHeight, 1.0f))
				->resetColor()->resetMaterial()
				->inColor(vec4(1.0f, 1.0f, 0.0f, 1.0f))
				->inMaterial(1.0f, 0.0f, 0.0f, 1.0f);
	if (!tmp->initialize())
		return false;
	addElement(tmp);

	//strut
	tmp = Mesh::newCylinder(stacks, slices, strutRadiusInHead, strutRadiusInLeg, true)
				->translated(vec3(headToLeg, topOfLeg-(cylinderHeight/4.0f), 0.0f))
				->rotated(vec3(0.0f,0.0f,1.0f), 90.0f)
				->scaled(vec3(1.0f,headToLeg, 1.0f));
	addElement(tmp);
	if (!tmp->initialize())
		return false;

	return true;
}
void Rocket::draw(const mat4 &model)
{
	mat4 m = model;
	transform(m);

	head->draw(m);
	for(int i = 0; i< 4; i++)
	{
		m = glm::rotate(m, 90.0f, vec3(0.0f,1.0f,0.0f));
		DrawableGroup::draw(m);
	}
}

void Rocket::replaceHead(Drawable *newHead) {
	head->takeDown();
	delete head;
	head = newHead;
}

void Rocket::takeDown()
{
	head->takeDown();
	for (
		list<Drawable*>::const_iterator
		iterator = getElements()->begin(),
			end = getElements()->end();
		iterator != end;
		++iterator)
	{
		(*iterator)->takeDown();
	}
}

Rocket::~Rocket()
{
	delete head;
	list<Drawable *> *elements = getElements();
	while (elements->size() > 0) {
		delete elements->back();
		elements->pop_back();
	}
}
