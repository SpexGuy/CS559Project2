#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal;

uniform mat4 mvp;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;
uniform vec4 color;

flat out vec4 fragColor;
out vec3 normal;
out vec3 position;
//flat out vec3 light;

 

void main()
{
	//get the vertex color
	fragColor = color;
	normal = normalize(normal_matrix * vertex_normal);
	//color = normal is a cool shader.
	//color = normal;


	//Position is the vertex position in the modelspace.
	position = vec3(modelview_matrix * vec4(vertex_position,1.0));
	//position in space
	gl_Position = mvp * vec4(vertex_position, 1.0);
}
