#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection;

void main() {
	TexCoords = texCoords;

	// MVP Matrix
	vec4 newPosition = u_Projection * u_ViewMatrix * vec4(position, 1.0f);

	gl_Position = newPosition;
}