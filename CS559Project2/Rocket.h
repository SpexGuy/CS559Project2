#pragma once
#include "Transformable.h"
#include <vector>
#include "Mesh.h"

class Rocket: public TransformableGroup
{
public:
	/**
	 * Constructs a Rocket
	 */
	Rocket();
	virtual bool initialize();
	virtual void draw(glm::mat4 model);
	virtual void takeDown();

	/**
	 *	Replaces the Head of the rocket with a new Mesh.
	 */
	void replaceHead(Mesh *newHead);
 	~Rocket();	

protected:
	Mesh* head;
	int stacks;
	int slices;
	float radius;

};
