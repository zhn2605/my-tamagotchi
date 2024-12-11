#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    shaderProgram = glCreateProgram();

    // Open file -> code
    std::string vertexCode, fragmentCode, line = "";
    std::ifstream vertexFile(vertexPath.c_str());
    if (vertexFile.is_open()) {
        while (std::getline(vertexFile, line)) {
            vertexCode += line + '\n';
        }
        vertexFile.close();
    }

    std::ifstream fragmentFile(fragmentPath.c_str());
    if (fragmentFile.is_open()) {
        while (std::getline(fragmentFile, line)) {
            fragmentCode += line + '\n';
        }
        fragmentFile.close();
    }

    // Compile shader code
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* v_src = vertexCode.c_str();
    const char* f_src = fragmentCode.c_str();

    glShaderSource(vertexShader, 1, &v_src, nullptr);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    glShaderSource(fragmentShader, 1, &f_src, nullptr);
    glCompileShader(fragmentShader);
    checkCompileErrors(vertexShader, "FRAGMENT");

    // Attach shaders to shaderProgramObject
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Validate Program
    glValidateProgram(shaderProgram);

    // Clean up
    glDetachShader(shaderProgram, vertexShader | fragmentShader);
    glDeleteShader(vertexShader | fragmentShader);
}


void Shader::useProgram() {
    glUseProgram(shaderProgram);
}

void Shader::deleteProgram() {
    glDeleteProgram(shaderProgram);
}

GLint Shader::getUniformLocation(const std::string& name) {
    GLint u_locataion = glGetUniformLocation(shaderProgram, name.c_str());
    if (u_locataion >= 0) {
        return(u_locataion);
    }
    else {
        std::cout << "could not find location of " << name.c_str() << ". Misspelling?" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
}

void Shader::setUniformVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setUniformMat4(const std::string& name, const glm::mat4x4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::checkCompileErrors(GLuint shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}