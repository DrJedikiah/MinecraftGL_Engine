#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "btBulletDynamicsCommon.h"

class Camera
{
public:
	Camera( int width, int height, float far = 100.f, float near = 0.1f, float fov = 45.f);

	void SetPosition(glm::vec3 vector);
	void Translate(glm::vec3 vector);
	void RotateUp(float angle);
	void RotateRight(float angle);

	const glm::mat4 projectionMatrix() const;
	const glm::mat4 viewMatrix() const;

	glm::vec3 position() const ;
	glm::vec3 up() const;
	glm::vec3 forward() const;
	glm::vec3 right() const;

private:
	glm::vec3 m_position;
	glm::vec3 m_up;
	glm::vec3 m_forward;
	glm::vec3 m_right;
	
	glm::mat4 m_projectionMatrix;

	mutable glm::mat4 m_viewMatrix;
	mutable bool viewMatrixChanged = false;
};