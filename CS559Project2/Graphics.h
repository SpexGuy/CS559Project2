#pragma once
#include <vector>
#include "Shader.h"
#include <GL/freeglut.h>
#include <glm/glm.hpp>

#define RED    vec4(1.0f, 0.0f, 0.0f, 1.0f)
#define YELLOW vec4(1.0f, 1.0f, 0.0f, 1.0f)
#define GREEN  vec4(0.0f, 1.0f, 0.0f, 1.0f)
#define BLUE   vec4(0.0f, 0.0f, 1.0f, 1.0f)
#define BLACK  vec4(0.0f, 0.0f, 0.0f, 1.0f)
#define WHITE  vec4(1.0f, 1.0f, 1.0f, 1.0f)

/**
 * The Graphics class encapsulates all logic related to drawing 3D shapes
 */
class Graphics {
private:
	glm::mat4 projection;
	glm::mat4 view;
	glm::ivec2 size;
	glm::vec3 light;
	glm::vec4 color;

	Shader *solidShader;

	std::vector<glm::ivec3> circleTrigs;
	std::vector<glm::ivec3> squareTrigs;
	std::vector<glm::ivec2> lineSegs;
	GLuint squareVH;
	GLuint squareCH;
	GLuint circleVH;
	GLuint circleCH;
	GLuint lineVH;
	GLuint lineCH;

	static Graphics instance;

	/* creates a Graphics instance but DOES NOT INITIALIZE IT */
	Graphics();

	inline void setSize(const glm::ivec2 &size) {
		this->size = size;
	}

	void setupShader(const Shader *s, const glm::mat4 &model) const;

public:
	/* Graphics is a Singleton */
	static Graphics *inst();

	/* should be called once a window context has been created.
	 * No other methods should be called until this has been. */
	bool initialize();

	bool loadBuffer(GLuint *vertexHandle, GLuint *coordinateHandle,
		GLsizeiptr size, const GLvoid *pointer);

	/* draws a wireframe cube from (-1, -1, -1) to (1, 1, 1),
	 * using legacy GL */
	void drawWireCube() const;

	/* draws the given text from the given base position translated
	 * by x and y with the given size (in pixels).
	 * MODIFIES THE MODELVIEW MATRIX */
	void drawText2D(const glm::mat4 &base, float x, float y, char *str, float size) const;

	void drawRect2D(const glm::mat4 &base, glm::vec2 blPoint, glm::vec2 trPoint) const;
	void drawRect2D(const glm::mat4 &base, float x, float y, float width, float height) const;

	void drawCircle2D(const glm::mat4 &base, glm::vec2 center, float radius) const;
	void drawCircle2D(const glm::mat4 &base, float x, float y, float radius) const;

	void drawLine2D(const glm::mat4 &base, glm::vec2 tlPoint, glm::vec2 brPoint) const;

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

	void drawPoints(const std::vector<int> &points,
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
	inline void setColor(const glm::vec4 &color) {
		this->color = color;
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
	inline glm::vec4 getColor() const {
		return color;
	}
};