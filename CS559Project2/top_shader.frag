#version 400

layout (location = 0) out vec4 FragColor;

uniform vec3 light_position;

//The lower the bigger the shine
uniform float shininess;

//these shoud add to (1, 1, 1)
uniform vec3 ambientScale;
uniform vec3 diffuseScale;
uniform vec3 specularScale;

flat in vec4 fragColor;
in vec3 position;
in vec3 normal;


vec3 ads()
{
  vec3 color = vec3(fragColor);
  vec3 n = normal;

  if (!gl_FrontFacing)
	n = -n;

  vec3 s = normalize(light_position - position);
  vec3 v = normalize(-position);
  vec3 r = reflect(-s, n);
  float s_dot_n = max(dot(s, n), 0.0);

  //return color * s_dot_n + (s_dot_n > 0 ? color * pow(max(dot(r, v), 0.0), shininess) : vec3(0.0));
  return
	ambientScale*color +
	diffuseScale*color*s_dot_n +
	(s_dot_n > 0 ? specularScale*color*pow(max(dot(r, v), 0.0), shininess) : vec3(0.0));
}

void main() {
	FragColor = vec4(ads(), 1.0);
}
