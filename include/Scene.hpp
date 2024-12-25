#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "mesh.hpp"
#include "model.hpp"

class Scene {
public:
	Scene();

	Model* CreateModel(const std::string name, const std::string& filepath);

	Model* GetObject(const std::string name);
	void PrepareDraw(int width, int height);
	
	void DrawObjects(const glm::mat4& view, const glm::mat4& projection, Shader* graphics, Shader* light);
	void CleanUpAll();
	
	/*
	void DrawLightSources(const glm::mat4& view, const glm::mat4& projection, Shader* lightShader);
	void UpdateAll();
	void CleanUpAll();
	*/

	void SetShaderProgram(GLuint shader);
	glm::vec3 m_bgColor = glm::vec3(0.0f);
private:
	std::string m_name;
	std::vector<std::unique_ptr<Model>> m_objects;
	//std::vector<std::unique_ptr<Mesh3D>> m_lightSources;
	GLuint m_shaderProgram;
};


#endif
