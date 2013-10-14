#pragma once
#include <GL/freeglut.h>
#include <glm/glm.hpp>

/**
 * The Graphics class encapsulates all logic related to drawing 3D shapes
 */
class Graphics {
private:
	GLuint sphereHandle;
	GLuint wireCubeHandle;
	GLuint wireSphereHandle;
	GLuint cylinderHandle;
	static GLsizei numHandles;
public:
	const static Graphics inst;

	/* creates a Graphics instance but DOES NOT INITIALIZE IT */
	Graphics();
	
	/* should be called once a window context has been created.
	 * No other methods should be called until this has been. */
	bool initialize();

	/* draws a sphere centered at the origin with radius 1 */
	void drawSphere() const;
	
	/* draws a wireframe sphere centered at the origin with radius 1 */
	void drawWireSphere() const;
	
	/* draws a wireframe cube from (-1, -1, -1) to (1, 1, 1) */
	void drawWireCube() const;
	
	/* draws a cylinder with radius 1 around the line segment from (0, 0, 0) to (0, 0, 1).
	 * Does not draw end caps. */
	void drawCylinder() const;

	/* draws the given text from the given base position translated
	 * by x and y with the given size (in pixels).
	 * MODIFIES THE MODELVIEW MATRIX */
	void drawText2D(const glm::mat4 &base, float x, float y, char *str, float size) const;

	~Graphics();
};