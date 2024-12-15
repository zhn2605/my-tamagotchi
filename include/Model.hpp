#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.hpp"
#include "Mesh.hpp"
using namespace std;

class Model {
public:
	Model(const string &path, bool gamma = false);

	void Draw(Shader& graphics, Shader& light);
	void SetName(const std::string name) { m_name = name; }
	void CleanUp();
	
	// Transform setters
	void SetPosition(const glm::vec3& position) {
		m_modelMatrix = glm::translate(glm::mat4(1.0f), position);
	}

	void SetScale(const glm::vec3& scale) {
		m_modelMatrix = glm::scale(m_modelMatrix, scale);
	}

	void SetRotation(float angle, const glm::vec3& axis) {
		m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(angle), axis);
	}

	void SetLightEmitter(bool isLightEmitter) {
		m_isLightEmitter = isLightEmitter;
	}
	
	glm::mat4 GetModelMatrix() { return m_modelMatrix; }
	bool IsLightEmitter() { return m_isLightEmitter; }
	std::string GetName() { return m_name; }
	bool gammaCorrection;
private:
	vector<Texture> textures_loaded; // already loaded textures
	vector<Mesh> meshes;
	string directory;
	string m_name;
	bool m_isLightEmitter = false;
	glm::mat4 m_modelMatrix = glm::mat4(1.0f);

	void loadModel(string const& path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	Material loadMaterialColors(aiMaterial* mat);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
	unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);
};

#endif