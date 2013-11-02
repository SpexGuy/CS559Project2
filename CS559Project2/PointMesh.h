#pragma once
#include <vector>
#include "Drawable.h"
#include "Transformable.h"
#include "Camera.h"
#include "Vertex.h"
#include <glm/glm.hpp>

class PointMesh : public Drawable, public TransformableMixin {
private:
	PointMesh();
protected:
	GLuint vertex_coordinate_handle;
	GLuint vertex_array_handle;
	Shader *starShader;
	std::vector<glm::vec3> points;
	std::vector<int> indexMask;
public:
	PointMesh(std::vector<glm::vec3> points);
	
	virtual bool initialize();

	//@Override
	virtual void draw(const glm::mat4 &model);

	virtual void takeDown();

	virtual ~PointMesh();

	//------------- Static Members --------------
	static PointMesh *newStarField(int numPoints, float radius);
};


class StarField : public PointMesh {
private:
	StarField();
public:
	StarField(std::vector<glm::vec3> points) : PointMesh(points) {}

	//@Override
	virtual void draw(const glm::mat4 &model);
};
