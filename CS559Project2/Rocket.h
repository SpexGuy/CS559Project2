#pragma once
#include "Transformable.h"
#include <vector>
#include "Mesh.h"

class Rocket: public TransformableGroup
{
public:
	Rocket();
	virtual bool initialize();
	virtual void draw(glm::mat4 model);
	void takeDown();
 	~Rocket();	

protected:
	Mesh* head;
	float stacks;
	float slices;

};
