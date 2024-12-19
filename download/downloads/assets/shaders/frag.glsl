#version 410 core
out vec4 color;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 VertexColors;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
	// ambient
	vec3 ambient = light.ambient * material.ambient;
	// vec3 ambient = vec3(0.3f, 0.2f, 0.4f);

	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * material.diffuse;
	// vec3 diffuse = vec3(1.0f, .4f, .2f);

	// specular
	vec3 viewDir = normalize(viewPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);
	// vec3 specular = vec3(1.0f, 1.0f, 1.0f);

	vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0);
}