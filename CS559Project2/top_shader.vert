#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;
//layout (location = 2) in vec3 vertex_color;

uniform mat4 mvp;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;

flat out vec3 color;
out vec3 normal;
out vec3 position;

void main()
{
	//get the vertex color
	color = vec3(1.0, 0.0, 0.0);
	normal = normalize(normal_matrix * vertex_normal);

	//Position is the vertex position in the modelspace.
	position = vec3(modelview_matrix * vec4(vertex_position,1.0));
	//position in space
	gl_Position = mvp * vec4(vertex_position, 1.0);
}
