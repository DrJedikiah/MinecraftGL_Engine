#include "engine/Camera.h"

Camera::Camera(int width, int height, float far, float near, float fov) :


	m_projectionMatrix(glm::perspective(glm::radians(fov), (float)width / (float)height, near, far)),
	m_position(0,0,0),
	m_up(0,1,0),
	m_forward(1, 0, 0),
	m_right( glm::cross(m_forward, m_up)),
	m_viewMatrix(glm::lookAt(m_position, m_position + m_forward,m_up))
{	

}

void Camera::SetPosition(glm::vec3 vector)
{
	m_position = vector;
	viewMatrixChanged = true;
}

void Camera::Translate(glm::vec3 vector)
{
	m_position += vector;
	viewMatrixChanged = true;
}

void Camera::RotateUp(float angle )
{
	m_forward = glm::angleAxis(angle, m_right) * m_forward;
	viewMatrixChanged = true;
}

void Camera::RotateRight(float angle)
{
	m_forward = glm::angleAxis(-angle, m_up) * m_forward;
	m_right = glm::cross(m_forward, m_up);
	viewMatrixChanged = true;
}

const glm::mat4 Camera::projectionMatrix() const { return m_projectionMatrix; }
glm::vec3 Camera::position() const {return m_position;}
glm::vec3 Camera::up() const { return m_up; }
glm::vec3 Camera::forward() const { return m_forward; }
glm::vec3 Camera::right() const { return m_right; }

const glm::mat4 Camera::viewMatrix() const
{
	if (viewMatrixChanged)
	{
		viewMatrixChanged = false;
		m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_up);
	}
	return m_viewMatrix;
}