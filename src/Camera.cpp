#define GLM_ENABLE_EXPERIMENTAL

#include "Camera.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include <iostream>

Camera::Camera() {
    m_eye = glm::vec3(0.0f, 0.0f, 0.0f);
    m_lookDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    m_fovy = 60.0f;
    m_aspect = 1.0f;
    m_near = 0.01f;
    m_far = 50.0f;
}

void Camera::UpdateAspectRatio(float aspect) {
    m_projectionMatrix = glm::perspective(m_fovy, aspect, m_near, m_far);
}

void Camera::MouseLook(int mouseX, int mouseY) {
    std::cout << "mouse: " << mouseX << ", " << mouseY << std::endl;

    glm::vec2 currentMouse = glm::vec2((float)mouseX, (float)mouseY);

    static bool firstLook = true;
    if (firstLook) {
        m_oldMousePos = currentMouse;
        firstLook = false;
    }

    glm::vec2 mouseDelta = m_oldMousePos - currentMouse;

    // Horizontal Look
    m_lookDirection = glm::rotate(m_lookDirection, .1f * glm::radians(mouseDelta.x), m_upVector);

    // Vertical Look
    glm::vec3 rightVector = glm::normalize(glm::cross(m_lookDirection, m_upVector));
    m_lookDirection = glm::rotate(m_lookDirection, .1f * glm::radians(mouseDelta.y), rightVector);

    m_lookDirection = glm::normalize(m_lookDirection);

    m_oldMousePos = currentMouse;
}

void Camera::MoveForward(float speed) {
    m_eye += (m_lookDirection * speed);
}

void Camera::MoveBackward(float speed) {
    m_eye -= (m_lookDirection * speed);
}

void Camera::MoveLeft(float speed) {
    glm::vec3 rightVector = glm::normalize(glm::cross(m_lookDirection, m_upVector));
    m_eye -= (rightVector * speed);
}

void Camera::MoveRight(float speed) {
    glm::vec3 rightVector = glm::normalize(glm::cross(m_lookDirection, m_upVector));
    m_eye += (rightVector * speed);
}

void Camera::MoveUp(float speed) {
    m_eye.y += glm::normalize(m_upVector).y * speed;
}

void Camera::MoveDown(float speed) {
    m_eye.y -= glm::normalize(m_upVector).y * speed;
}

// Setters
void Camera::SetProjectionMatrix(float fovy, float aspect, float near, float far) {
    m_fovy = fovy;
    m_near = near;
    m_aspect = aspect;
    m_far = far;
    m_projectionMatrix = glm::perspective(fovy, aspect, near, far);
}

void Camera::SetFovy(float fovy) {
    m_fovy = fovy;
    m_projectionMatrix = glm::perspective(glm::radians(fovy), m_aspect, m_near, m_far);
}

// Getters
float Camera::GetFovy() { return m_fovy; }

glm::vec3 Camera::GetEye() { return m_eye; }
glm::vec3 Camera::GetLookDir() { return m_lookDirection; }

glm::mat4 Camera::GetProjectionMatrix() const {
    return m_projectionMatrix;
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(m_eye, m_eye + m_lookDirection, m_upVector);
}