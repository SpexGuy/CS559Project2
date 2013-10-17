#version 400

layout (location = 0) out vec4 FragColor;

flat in vec3 color;
in vec3 position;
in vec3 normal;
uniform vec3 light_position;

//The lower the bigger the shine
const float shininess = 1000.0f;

vec3 ads()
{
  vec3 n = normal;

  if (!gl_FrontFacing)
	n = -n;

  vec3 s = normalize(light_position - position);
  vec3 v = normalize(-position);
  vec3 r = reflect(-s, n);
  float s_dot_n = max(dot(s, n), 0.0);

  return color * s_dot_n;
}

void main()
{
	FragColor = vec4(ads(), 1.0);
}
