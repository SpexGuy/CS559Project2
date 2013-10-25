#include <vector>
#include <glm\glm.hpp>
#include "Rocket.h"
#include "Mesh.h"

using namespace glm;
using namespace std;

Rocket::Rocket()
{
	 this->stacks = 20;
	 this->slices = 20;
}

bool Rocket::initialize()
{
	float headHeight = 5.0f;
	float headradius = 3.0f;
	float strutLength = 0.25f;
	float cylinderHeight = 1.25f;
	float landingGearHeight = 3.0f;
	float strutSmallR = 0.25f;
	float strutBigR = 0.25f;

//tanslate rotate scale in that order

	//Draws the head
	head = Mesh::newSphere(stacks, slices, headradius, true);
	if (!head->initialize())
		return false;
	head->position(vec3(0.0f));
	head->scale(vec3(1.0f,headHeight,1.0f));

	/*
	
	//top sphere
	Mesh* tmp = Mesh::newSphere(stacks, slices, 1.0f,true);
	if (!tmp->initialize())
		return false;
	tmp->position(vec3(strutLength, 0.0f, -(headHeight/1.5f)));
	addElement(tmp);

	//mid cylinder
	tmp = Mesh::newCylinder(stacks, slices, 1.0f, 1.0f, 1.0f, true);
	if (!tmp->initialize())
		return false;
	tmp->position(vec3(strutLength, -(headHeight/1.5f)-(cylinderHeight/2.0f), 0.0f));
	tmp->scale(vec3(1.0f,cylinderHeight, 1.0f));
	addElement(tmp);

	//bottom sphere
	tmp = Mesh::newSphere(stacks, slices, 1.0f,true);
	if (!tmp->initialize())
		return false;
	tmp->position(vec3(strutLength, -(headHeight/1.5f)-(cylinderHeight), 0.0f));
	tmp->scale(vec3(1.0f,cylinderHeight, 1.0f));
	addElement(tmp);

	//strut
	tmp = Mesh::newCylinder(stacks, slices, 1.0f, strutBigR, strutSmallR, true);
	if (!tmp->initialize())
		return false;
	tmp->position(vec3(strutLength/2.0f, -(headHeight/1.5f)-(cylinderHeight/0.25f), 0.0f));
	tmp->setRotation(vec3(1.0f,0.0f,0.0f), 90.0f);
	tmp->scale(vec3(1.0f,cylinderHeight, 1.0f));
	addElement(tmp);
	*/

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
