/*	Perry Kivolowitz - University of Wisconsin - Madison 
	Computer Sciences Department

	A sample hello world like program demonstrating modern
	OpenGL techniques. 

	Created:	2/25/13
	Updates:
*/

#pragma once
#include <map>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderFlyweight;

class Shader
{
public:
	void use() const;
	virtual void customSetup() const;
	void commonSetup(const float time, const GLint *size, const GLfloat *projection, const GLfloat *modelview, const GLfloat *mvp, 
					const GLfloat *nm, const GLfloat *light, const GLfloat *color,
					const GLfloat *ambient, const GLfloat *diffuse, const GLfloat *specular, const float shiny) const;

	std::stringstream getShaderLog(GLuint shader_id);

	friend class ShaderFlyweight;

protected:
	GLuint modelview_matrix_handle;
	GLuint projection_matrix_handle;
	GLuint normal_matrix_handle;
	GLuint mvp_handle;
	GLuint size_handle;
	GLuint time_handle;
	GLuint light_handle;
	GLuint color_handle;
	GLuint ambient_handle;
	GLuint diffuse_handle;
	GLuint specular_handle;
	GLuint shiny_handle;

	GLuint vertex_shader_id;
	GLuint fragment_shader_id;
	GLuint program_id;

	Shader();
	virtual bool initialize(char * vertex_shader_file, char * fragment_shader_file);
	bool loadShader(const char * file_name, GLuint shader_id);
	void takeDown();
	bool GLReturnedError(char * s) const;

};

class BackgroundShader : public Shader
{
protected:
	GLuint color_array_handle;

	BackgroundShader();
	virtual bool initialize(char * vertex_shader_file, char * fragment_shader_file);
	virtual void customSetup(glm::vec4 * color_array);

private:
	typedef Shader super;
};

#define SHADER_SOLID 0
#define SHADER_ADS 1
#define SHADER_STAR 2

class ShaderFlyweight {
public:
	/* ShaderFlyweight is a Singleton */
	static ShaderFlyweight instance;
	static ShaderFlyweight *inst();
	Shader *getShader(int name);
	Shader *loadShader(int name, char *vertFile, char *fragFile);
	void addShader(int name, Shader *shader);
	bool initialize();
	void takeDown();

private:
	std::map<int, Shader *> loadedShaders;
	ShaderFlyweight();
	~ShaderFlyweight();

};
