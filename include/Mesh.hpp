#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"

#include <string>
#include <vector>
using namespace std;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	string type;
};

class Mesh {
public:
	vector<Vertex> m_vertices;
	vector<unsigned int> m_indices;
	vector<Texture> m_textures;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indicies, vector<Texture> textures);
	void Draw(Shader &shader);

private:
	unsigned int VAO, VBO, EBO;

	void Initialize();
};


#endif
