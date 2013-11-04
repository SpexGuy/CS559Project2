#pragma once
#include <vector>
#include "Drawable.h"
#include "Transformable.h"
#include "Camera.h"
#include "Vertex.h"
#include <glm/glm.hpp>

/** A drawable made up of points */
class PointMesh : public Drawable {
private:
	PointMesh();
protected:
	GLuint vertex_coordinate_handle;
	GLuint vertex_array_handle;
	Shader *starShader;
	std::vector<glm::vec3> points;
	std::vector<int> indexMask;
public:
	PointMesh(const std::vector<glm::vec3> &points);
	
	virtual bool initialize();

	//@Override
	virtual void draw(const glm::mat4 &model);

	virtual void takeDown();

	virtual ~PointMesh();

	//------------- Static Members --------------

	/* creates a star field of the given radius with the given
	 * number of stars */
	static PointMesh *newStarField(const int &numPoints, const float &radius);
};
