#pragma once
#include <vector>
#include <glm\glm.hpp>
#include "Transformable.h"
#include "Mesh.h"

class  ComplexObject : public Transformable 
{
public:
	static ComplexObject *newRocket();
	~ ComplexObject();

	void draw();

private:
	//way a
	ComplexObject();

	//way b
	//This would iterate through the meshs and assign each on a matrix to be draw in
	//Shapes are draw at the center
	ComplexObject(std::vector<Mesh> , std::vector<glm::mat4>);

};
