#include "Scene.hpp"

Scene::Scene() {
    m_shaderProgram = 0;
}

void Scene::SetShaderProgram(GLuint shader) {
    m_shaderProgram = shader;
}

Model* Scene::CreateModel(const std::string name, const std::string &filepath) {
    auto obj = std::make_unique<Model>(filepath);
    obj->SetName(name);

    Model* ptr = obj.get();
    m_objects.push_back(std::move(obj));
    return ptr;
}

Model* Scene::GetObject(const std::string name) {
    for (const auto& obj : m_objects) {
        if (obj->GetName() == name) {
            return obj.get();
        }
    }

    std::cerr << "Object not found in scene" << std::endl;
    return nullptr;
}

void Scene::PrepareDraw(int width, int height) {
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, width, height);        // creates a viewport starting left corner (0,0) 
    //glClearColor(0.2f, 0.3f, .3f, 1.0f);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_shaderProgram);        // modifying shaders in program object will not affect curr executables

}

void Scene::DrawObjects(const glm::mat4 &view, const glm::mat4 &projection, Shader *shader) {
    // Set view and projection matrices
    GLint viewLocation = glGetUniformLocation(m_shaderProgram, "u_ViewMatrix");
    GLint projLocation = glGetUniformLocation(m_shaderProgram, "u_Projection");
    if (viewLocation >= 0) glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
    if (projLocation >= 0) glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);

    for (const auto& obj : m_objects) {
        GLint modelLocation = glGetUniformLocation(m_shaderProgram, "u_ModelMatrix");
        if (modelLocation >= 0) {
            glm::mat4 model = obj->GetModelMatrix();
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, &model[0][0]);
        }
        // Draw objet
        obj->Draw(*shader);
    }
}