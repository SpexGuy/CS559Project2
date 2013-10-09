#include <vector>
#include <glm/glm.hpp>

class Mesh {
private:
	Mesh();
protected:
	std::vector<glm::vec3> *points;
	std::vector<glm::ivec3> *trigs;
	std::vector<glm::vec3> *normals;
public:
	/** generates a mesh with the given points. Trigs is a vector of
	 * triangles.  A triangle is a vec3 of indexes in <points>
	 * calculates triangle normals by taking the vector from trig[1]
	 * to trig[2] and crossing it with the vector from trig[1] to trig[0].
	 * Calculates point normals by averaging the vectors from all triangles
	 * which have this point as a vertex. */
	Mesh(std::vector<glm::vec3> *points, std::vector<glm::ivec3> *trigs);

	/** creates a spherical Mesh with a base radius offset by data
	 * from the given file, scaled by radScale.
	 *
	 *
	 * The file format is as follows:
	 * The first line is the width followed by the height of the data,
	 * separated by a tab. There should be exactly <height> more lines,
	 * each consisting of <width> float values between 0 and 1, separated
	 * by tabs. */
	static Mesh *newMars(float radius, float radScale,
		char *filename);

	/** creates a spherical mesh with the radius at point i,j
	 * calculated as radius + radScale*radii[i][j]. 
	 */
	static Mesh *newMars(float radius, float radScale,
		std::vector<std::vector<float>> radii);

	virtual ~Mesh();

};