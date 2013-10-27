#version 400

layout (location = 0) in vec3 vertex_position;

uniform mat4 mvp;
flat out vec4 fragColor;

/** this function is adapted from http://www.ozone3d.net/blogs/lab/20110427/glsl-random-generator/
 * on StackOverflow.com:
 * http://www.ozone3d.net/blogs/lab/20110427/glsl-random-generator/
 */
float rand(vec3 n)
{
  return 0.5 + 0.5 * 
     fract(sin(dot(n.xyz, vec3(12.9898, 78.233, 5.989)))* 43758.5453);
}
void main()
{
	gl_Position = mvp * vec4(vertex_position, 1.0);

	float a = rand(vertex_position.xyz);

	//yellow-white
	fragColor = vec4(2.0f*a,2.0f*a,a,1.0f);

}
