#pragma once
#include <vector>
#include "shader.h"
#include <GL/freeglut.h>
#include <glm/glm.hpp>

/**
 * The Graphics class encapsulates all logic related to drawing 3D shapes
 */
class Graphics {
private:
	glm::mat4 projection;
	glm::mat4 view;	

	static GLsizei numHandles;

	static Graphics instance;

	/* creates a Graphics instance but DOES NOT INITIALIZE IT */
	Graphics();

public:
	/* Graphics is a Singleton */
	static Graphics *inst();

	/* should be called once a window context has been created.
	 * No other methods should be called until this has been. */
	bool initialize();

	/* draws a wireframe cube from (-1, -1, -1) to (1, 1, 1) */
	void drawWireCube() const;

	/* draws the given text from the given base position translated
	 * by x and y with the given size (in pixels).
	 * MODIFIES THE MODELVIEW MATRIX */
	void drawText2D(const glm::mat4 &base, float x, float y, char *str, float size) const;

	void drawTriangles(const std::vector<glm::ivec3> &trigs,
					   const GLuint &vertexArrayHandle,
					   const Shader &s,
					   const glm::mat4 &model)
				const;

	/* frees resources */
	void takeDown();

	/* asserts that takeDown() has been called */
	~Graphics();

	//getters and setters
	inline void setProjection(glm::mat4 &projection) {
		this->projection = projection;
	}
	inline void setView(glm::mat4 &view) {
		this->view = view;
	}
	inline glm::mat4 getProjection() {
		return projection;
	}
	inline glm::mat4 getView() {
		return view;
	}
};