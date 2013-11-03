#version 400

uniform sampler2D textureIndex;

uniform vec3 light_position;

//The lower the bigger the shine
uniform float shininess;

//these shoud add to (1, 1, 1, 1)
uniform vec3 ambientScale;
uniform vec3 diffuseScale;
uniform vec4 specularColor;

in vec3 position;
in vec3 normal;
in vec2 UV;
 
out vec4 FragColor;

vec4 ads() {
  vec3 color = texture( textureIndex, UV ).rgb;
  vec3 n = normalize(normal);

  if (!gl_FrontFacing)
	n = -n;

  vec3 s = normalize(light_position - position);
  float s_dot_n = max(dot(s, n), 0.0);

  vec3 specular = vec3(0.0);
  vec3 v;
  vec3 r;
  if (specularColor.w != 0) {
	v = normalize(-position);
	r = reflect(-s, n);
	specular = (s_dot_n > 0 ? vec3(specularColor)*pow(max(dot(r, v), 0.0), shininess) : vec3(0.0));
  }

  return vec4(
	ambientScale*color +
	diffuseScale*color*s_dot_n +
	specular,
	1.0);
}

void main(){
    // Output color = color of the texture at the specified UV
    FragColor = ads();
}