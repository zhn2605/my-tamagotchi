#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.hpp"
#include "Mesh.hpp"
using namespace std;

class Model {
public:
	Model(const string& path);

	void Draw(Shader& shader); 

private:
	vector<Texture> textures_loaded; // already loaded textures
	vector<Mesh> meshes;
	string directory;

	void loadModel(string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
	unsigned int TextureFromFile(const char* path, const string& directory);
};

#endif