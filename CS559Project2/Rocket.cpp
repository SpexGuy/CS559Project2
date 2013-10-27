#include <vector>
#include <glm\glm.hpp>
#include "Rocket.h"
#include "Mesh.h"

using namespace glm;
using namespace std;

Rocket::Rocket()
{
	 this->stacks = 32;
	 this->slices = 32;
}

bool Rocket::initialize()
{
	float headHeight = 2.0f;
	float headradius = 1.0f;
	float headToLeg = headradius*2.0f;
	float topOfLeg = -headHeight/1.625f;
	float cylinderHeight = headHeight/1.25f;
	float smallRadius = headradius/4.0f;
	float structRadiusInLeg = smallRadius/2.0f;
	float structRadiusInHead = smallRadius*1.25f;

//tanslate rotate scale in that order

	//Draws the head
	head = Mesh::newSphere(stacks, slices, headradius, true);
	if (!head->initialize())
		return false;
	head->position(vec3(0.0f));
	head->scale(vec3(1.0f,headHeight,1.0f));

	//top sphere
	Mesh* tmp = Mesh::newSphere(stacks, slices, smallRadius, true);
	if (!tmp->initialize())
		return false;
	tmp->position(vec3(headToLeg, topOfLeg, 0.0f));
	addElement(tmp);

	//mid cylinder
	tmp = Mesh::newCylinder(stacks, slices, smallRadius, smallRadius, true);
	if (!tmp->initialize())
		return false;
	tmp->position(vec3(0.0f, topOfLeg, headToLeg));
	tmp->setRotation(vec3(0.0f,0.0f,1.0f), 180.0f);
	tmp->scale(vec3(1.0f,cylinderHeight, 1.0f));

	addElement(tmp);

	//bottom sphere
	tmp = Mesh::newSphere(stacks, slices, smallRadius,true);
	if (!tmp->initialize())
		return false;
	tmp->position(vec3(headToLeg, topOfLeg-cylinderHeight, 0.0f));
	tmp->scale(vec3(1.0f,cylinderHeight, 1.0f));
	addElement(tmp);

	//strut
	tmp = Mesh::newCylinder(stacks, slices, structRadiusInHead, structRadiusInLeg, true);
	if (!tmp->initialize())
		return false;
	tmp->position(vec3(headToLeg, topOfLeg-(cylinderHeight/4.0f), 0.0f));
	tmp->setRotation(vec3(0.0f,0.0f,1.0f), 90.0f);
	tmp->scale(vec3(1.0f,headToLeg, 1.0f));
	addElement(tmp);

	return true;
}
void Rocket::draw(mat4 model)
{
	transform(model);

	head->draw(model);
	for(int i = 0; i< 4; i++)
	{
		model = glm::rotate(model, 90.0f, vec3(0.0f,1.0f,0.0f));
		DrawableGroup::draw(model);

	}
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
		((Mesh*)(*iterator))->takeDown();
	}
}

Rocket::~Rocket()
{
	delete head;
}
