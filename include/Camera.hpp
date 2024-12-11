#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera{
public:
	
	Camera();

	void UpdateAspectRatio(float aspect);

	void MouseLook(int mouseX, int mouseY);
	void MoveForward(float speed);
	void MoveBackward(float speed);
	void MoveLeft(float speed);
	void MoveRight(float speed);
	void MoveUp(float speed);
	void MoveDown(float speed);

	void SetProjectionMatrix(float fovy, float aspect, float near, float far);
	void SetFovy(float fovy);

	float GetFovy();
	glm::vec3 GetEye();
	glm::vec3 GetLookDir();
	glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetViewMatrix() const;

private:
	glm::mat4 m_projectionMatrix;

	glm::vec3 m_eye;
	glm::vec3 m_lookDirection;
	glm::vec3 m_upVector;

	float m_fovy;
	float m_aspect;
	float m_near;
	float m_far;

	glm::vec2 m_oldMousePos;	// store old mouse pos
};

#endif