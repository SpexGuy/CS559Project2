#pragma once
#include <vector>
#include "Shader.h"
#include "Vertex.h"
#include "Transformable.h"
#include <glm/glm.hpp>

class Mesh : public Drawable, public TransformableMixin {
private:
	Mesh();
protected:

	bool drawNormals;
	GLuint vertex_coordinate_handle;
	GLuint vertex_array_handle;
	GLuint normal_coordinate_handle;
	GLuint normal_array_handle;

	Shader *shader;
	Shader *solidShader;
	std::vector<VertexPN> points;
	std::vector<glm::ivec3> trigs;
	std::vector<glm::vec3> normPoints;
	std::vector<glm::ivec2> normSegs;
	

public:
	/** generates a mesh with the given points. Trigs is a vector of
	 * triangles.  A triangle is a vec3 of indexes in <points>
	 * calculates triangle normals by taking the vector from trig[1]
	 * to trig[2] and crossing it with the vector from trig[1] to trig[0].
	 * Calculates point normals by averaging the vectors from all triangles
	 * which have this point as a vertex. */
	Mesh(std::vector<glm::vec3> points, std::vector<glm::ivec3> trigs);

	virtual bool initialize();
	
	virtual void draw(const glm::mat4 &modelview);

	virtual void takeDown();

	inline void setDrawNormals(bool dn) {
		this->drawNormals = dn;
	}
	inline bool isDrawingNormals() {
		return drawNormals;
	}

	virtual ~Mesh();



	//---------Static Members------------------------

	/** creates a spherical Mesh with a base radius offset by data
	 * from the given file, scaled by radScale.
	 *
	 * The file format is as follows:
	 * The first line is the width followed by the height of the data,
	 * separated by a tab. There should be exactly <height> more lines,
	 * each consisting of <width> float values between 0 and 1, separated
	 * by tabs.
	 * Returns */
	static Mesh *newMars(float radius, float radScale,
		char *filename, bool crosshatch = false);

	/** creates a spherical mesh with the radius at point i,j
	 * calculated as radius + radScale*radii[i][j]. 
	 */
	static Mesh *newMars(float radius, float radScale,
		std::vector<std::vector<float>> radii, bool crosshatch = false);

	/** creates a cylinder mesh with a certain number of stacks and slices,
	 *  a top radius and bottom radius. The origin of the cylinder is the 
	 *  center of the top slice. 
	 */
	static Mesh *newCylinder(int stacks, int slices, 
				float topRadius, float botRadius, bool crosshatch = false);

	/** creates a sphere mesh with a certain number of stacks and slices,
	 * along with a radius. The origin of the sphere is center of the sphere.
	 */
	static Mesh *newSphere(int stacks, int slices, float radius, bool crosshatch = false);

	/** creates a surface of rotation mesh with a certain number of slices,
	 * the stacks are that are is based off the points passed in.  
	 */
	static Mesh *newSurfaceOfRotation(const std::vector<glm::vec2> &points, int slices, bool crosshatch = false);

	/** creates a vector of pairs of indices that coorespond to the vector of points to discribe triangles. 
	 */
	static std::vector<glm::ivec3> generateTrigs(std::vector<glm::vec3> points,
			int width, int height, bool endcaps, bool crosshatch);
};