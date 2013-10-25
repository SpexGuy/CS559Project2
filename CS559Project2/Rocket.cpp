#include <vector>
#include <glm\glm.hpp>
#include "Rocket.h"
#include "Mesh.h"

using namespace glm;
using namespace std;

Rocket::Rocket()
{
	 this->stacks = 16;
	 this->slices = 16;
}

bool Rocket::initialize()
{
	float headHeight = 2.0f;
	float headradius = 1.0f;
	float smallSphereRadius = headradius/4.0f;
	float headToLeg = 0.25f;
	float cylinderHeight = 1.25f;
	float landingGearHeight = 3.0f;
	float strutInLeg = smallSphereRadius/4.0f;
	float strutInHead = smallSphereRadius;

//tanslate rotate scale in that order

	//Draws the head
	head = Mesh::newSphere(stacks, slices, headradius,true);
	if (!head->initialize())
		return false;
	head->position(vec3(0.0f));
	head->scale(vec3(1.0f,headHeight,1.0f));
	addElement(head);
	
	//top sphere
	Mesh* tmp = Mesh::newSphere(stacks, slices, smallSphereRadius,true);
	if (!tmp->initialize())
		return false;
	tmp->position(vec3(headToLeg+smallSphereRadius+headradius, -(headHeight/1.5f), 0.0f));
	addElement(tmp);

	//mid cylinder
	tmp = Mesh::newCylinder(stacks, slices, smallSphereRadius, smallSphereRadius, true);
	if (!tmp->initialize())
		return false;
	tmp->position(vec3(headToLeg+smallSphereRadius+headradius, (-headHeight/1.5f), 0.0f));
	tmp->setRotation(vec3(0.0f,0.0f,1.0f), 180.0f);
	addElement(tmp);

	//bottom sphere
	tmp = Mesh::newSphere(stacks, slices, smallSphereRadius,true);
	if (!tmp->initialize())
		return false;
	tmp->position(vec3(headToLeg+smallSphereRadius+headradius, (-headHeight/1.5f)-cylinderHeight + smallSphereRadius, 0.0f));
	tmp->scale(vec3(1.0f,landingGearHeight, 1.0f));
	addElement(tmp);

	//strut
	tmp = Mesh::newCylinder(stacks, slices, strutInLeg, strutInHead, true);
	if (!tmp->initialize())
		return false;
	tmp->position(vec3(0, (-headHeight/1.5f)-(cylinderHeight/4.0f) + (strutInHead-strutInLeg)/2.0f, 0.0f));
	tmp->scale(vec3(1.0f,headToLeg+smallSphereRadius+headradius,1.0f));
	tmp->setRotation(vec3(0.0f,0.0f,1.0f), 90.0f);
	addElement(tmp);
	
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
