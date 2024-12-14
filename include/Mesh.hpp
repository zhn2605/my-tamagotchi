#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
		int m_BoneIDs[MAX_BONE_INFLUENCE];
		float m_Weights[MAX_BONE_INFLUENCE];
};

struct Material {
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	float Shininess;
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh {
public:
	vector<Vertex> m_vertices;
	vector<unsigned int> m_indices;
	vector<Texture> m_textures;
	Material m_material;
	unsigned int m_VAO;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indicies, Material material);
	void Draw(Shader &shader);
	void DrawColor(Shader& shader);

private:
	unsigned m_VBO, m_EBO;

	void Initialize();
};

#endif
