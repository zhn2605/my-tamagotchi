#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec3 texCoords;

out vec3 FragPos;
out vec2 Normal;
out vec2 TexCoords;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection;

void main() {
	FragPos = vec3(model * vec4(position), 1.0);
	Normal = mat3(transpose(inverse(u_ModelMatrix))) * normal;
	TexCoords = texCoords;

	// MVP Matrix
	vec4 newPosition = u_Projection * u_ViewMatrix * u_ModelMatrix * vec4(position, 1.0f);

	gl_Position = newPosition;
}