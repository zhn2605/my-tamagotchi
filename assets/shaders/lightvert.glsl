#version 410 core
layout (location=0) in vec3 position;

uniform mat4 u_ModelMatrix;

void main() {
	// MVP Matrix
	vec4 newPosition = u_ModelMatrix * vec4(position, 1.0f);

	gl_Position = newPosition;
}