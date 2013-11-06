#pragma once
#include <vector>
#include "Shader.h"
#include <GL/freeglut.h>
#include <glm/glm.hpp>

// Some common colors
#define RED    vec4(1.0f, 0.0f, 0.0f, 1.0f)
#define YELLOW vec4(1.0f, 1.0f, 0.0f, 1.0f)
#define GREEN  vec4(0.0f, 1.0f, 0.0f, 1.0f)
#define BLUE   vec4(0.0f, 0.0f, 1.0f, 1.0f)
#define BLACK  vec4(0.0f, 0.0f, 0.0f, 1.0f)
#define WHITE  vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define MARS   vec4(1.0f, 0.2f, 0.2f, 1.0f)

// modelview modes
#define MV_FULL 7
#define MV_ROTATION 2

/**
 * The Graphics class encapsulates all logic related to drawing 3D shapes
 * It also serves to accumulate state for the shaders.
 */
class Graphics {
private:

	glm::mat4 projection;
	glm::mat4 view;
	
	glm::ivec2 size;
	glm::vec3 light;
	glm::vec4 color;
	
	glm::vec4 specularColor;
	float ambient, diffuse, shininess;
	
	int modelviewMode;

	int texIndex;

	int time;

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

	/* configures the shader with the accumulated state */
	void setupShader(const Shader *s, const glm::mat4 &model) const;

public:
	/* Graphics is a Singleton */
	static Graphics *inst();

	/* should be called once a window context has been created.
	 * No other methods should be called until this has been. */
	bool initialize();

	/* loads the given buffer onto the graphics card
	 * will overwrite vertexHandle and coordinateHandle
	 * with generated values. */
	bool loadBuffer(GLuint *vertexHandle, GLuint *coordinateHandle,
		GLsizeiptr size, const GLvoid *data);

	/* draws a wireframe cube from (-1, -1, -1) to (1, 1, 1),
	 * using legacy GL */
	void drawWireCube() const;

	/* draws the given text from the given base position translated
	 * by x and y with the given size (in pixels). */
	void drawText2D(const glm::mat4 &base, float x, float y, char *str, float size) const;

	/* draws a rectangle with the given corners */
	void drawRect2D(const glm::mat4 &base, glm::vec2 blPoint, glm::vec2 trPoint) const;
	/* draws a rectangle at the given position with the given size */
	void drawRect2D(const glm::mat4 &base, float x, float y, float width, float height) const;

	/* draws a circle with the given center and radius */
	void drawCircle2D(const glm::mat4 &base, glm::vec2 center, float radius) const;
	/* draws a circle at the given coordinates with the given radius */
	void drawCircle2D(const glm::mat4 &base, float x, float y, float radius) const;

	/* draws a line between the two points */
	void drawLine2D(const glm::mat4 &base, glm::vec2 tlPoint, glm::vec2 brPoint) const;

	/* draws triangles with the given array and shader
	 * using the given model space */
	void drawTriangles(const std::vector<glm::ivec3> &trigs,
					   const GLuint &vertexArrayHandle,
					   const Shader *s,
					   const glm::mat4 &model)
				const;

	/* draws lines with the given array and shader
	 * using the given model space */
	void drawLines(const std::vector<glm::ivec2> &segs,
				   const GLuint &vertexArrayHandle,
				   const Shader *s,
				   const glm::mat4 &model)
				const;

	/* draws points with the given array and shader
	 * using the given model space */
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

	
	
	//---------------- getters and setters -----------------

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
	inline void setMaterial(const float &a, const glm::vec4 &s, const float &shiny) {
		assert(a <= 1);
		this->ambient = a;
		this->specularColor = s;
		this->shininess = shiny;
	}
	inline void setModelviewMode(int mode) {
		modelviewMode = mode;
	}
	inline void setTime(int time) {
		this->time = time;
	}
	inline void setTexture(int texIndex) {
		this->texIndex = texIndex;
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
	inline float getAmbient() const {
		return ambient;
	}
	inline float getDiffuse() const {
		return 1-ambient;
	}
	inline glm::vec4 getSpecularColor() const {
		return specularColor;
	}
	inline float getShininess() const {
		return shininess;
	}
	inline int getModelviewMode() const {
		return modelviewMode;
	}
	inline int getTime() const {
		return time;
	}
	inline int getTexture() const {
		return texIndex;
	}
};