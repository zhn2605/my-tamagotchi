#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texCoords;
layout(location=3) in vec3 color;

out vec3 FragPos;
out vec3 Normal;
out vec3 VertexColor;
out vec2 TexCoords;

uniform mat4 u_ModelMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_Projection;

void main() {
	FragPos = vec3(u_ModelMatrix * vec4(position, 1.0));
	Normal = mat3(transpose(inverse(u_ModelMatrix))) * normal;
	TexCoords = texCoords;

	// MVP Matrix
	vec4 newPosition = u_Projection * u_ViewMatrix * vec4(FragPos, 1.0);

	gl_Position = newPosition;
}
