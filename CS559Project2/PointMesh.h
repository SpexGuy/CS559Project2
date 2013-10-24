#pragma once
#include <vector>
#include "Drawable.h"
#include "Transformable.h"
#include <glm/glm.hpp>

class PointMesh : public Drawable, public TransformableMixin {
private:
	PointMesh();
protected:
	GLuint vertex_coordinate_handle;
	GLuint vertex_array_handle;
	Shader *solidShader;
	std::vector<glm::vec3> points;
	std::vector<int> indexMask;
public:
	PointMesh(std::vector<glm::vec3> points);

	virtual bool initialize();

	//@Override
	virtual void draw(glm::mat4 modelview);

	virtual void takeDown();

	virtual ~PointMesh();

};