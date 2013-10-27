#version 400

layout (location = 0) out vec4 FragColor;

flat in vec4 fragColor;

void main() {

	FragColor = fragColor;
}
