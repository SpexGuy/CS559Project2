#pragma once
#include <vector>
#include "Shader.h"
#include <GL/freeglut.h>
#include <glm/glm.hpp>

/**
 * The Graphics class encapsulates all logic related to drawing 3D shapes
 */
class Graphics {
private:
	glm::mat4 projection;
	glm::mat4 view;
	glm::ivec2 size;
	glm::vec3 light;

	static Graphics instance;

	/* creates a Graphics instance but DOES NOT INITIALIZE IT */
	Graphics();

	inline void setSize(const glm::ivec2 &size) {
		this->size = size;
	}

public:
	/* Graphics is a Singleton */
	static Graphics *inst();

	/* should be called once a window context has been created.
	 * No other methods should be called until this has been. */
	bool initialize();

	bool loadBuffer(GLuint *arrayHandle, GLuint *coordinateHandle,
		GLsizeiptr size, const GLvoid *pointer);

	/* draws a wireframe cube from (-1, -1, -1) to (1, 1, 1) */
	void drawWireCube() const;

	/* draws the given text from the given base position translated
	 * by x and y with the given size (in pixels).
	 * MODIFIES THE MODELVIEW MATRIX */
	void drawText2D(const glm::mat4 &base, float x, float y, char *str, float size) const;

	void drawTriangles(const std::vector<glm::ivec3> &trigs,
					   const GLuint &vertexArrayHandle,
					   const Shader *s,
					   const glm::mat4 &model)
				const;

	void drawLines(const std::vector<glm::ivec2> &segs,
				   const GLuint &vertexArrayHandle,
				   const Shader *s,
				   const glm::mat4 &model)
				const;

	/* generates a glViewport with the given parameters.
	 * also sets the size to (width, heigth). */
	void viewport(int x, int y, int width, int height);

	/* frees resources */
	void takeDown();

	/* asserts that takeDown() has been called */
	~Graphics();

	//getters and setters
	inline void setProjection(const glm::mat4 &projection) {
		this->projection = projection;
	}
	inline void setView(const glm::mat4 &view) {
		this->view = view;
	}
	inline void setLight(const glm::vec3 &light) {
		this->light = light;
	}

	inline glm::mat4 getProjection() const {
		return projection;
	}
	inline glm::mat4 getView() const {
		return view;
	}
	inline glm::ivec2 getSize() const {
		return size;
	}
	inline glm::vec3 getLight() const {
		return light;
	}
};