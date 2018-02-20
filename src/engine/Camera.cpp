#include "engine/Camera.h"

Camera::Camera(int width, int height, float far, float near, float fov) :


	m_projectionMatrix(glm::perspective(glm::radians(fov), (float)width / (float)height, near, far)),
	m_position(0,0,3),
	m_up(0,1,0),
	m_forward(glm::vec3(0, 0, -1)),
	m_right( glm::cross(m_forward, m_up)),
	m_viewMatrix(glm::lookAt(m_position, m_position + m_forward,m_up))
{	

}

void Camera::translate(glm::vec3 vector)
{
	m_position += vector;

	m_viewMatrix = glm::lookAt(
		m_position,
		m_position + m_forward ,
		m_up);
}

void Camera::translate(float x, float y, float z)
{
	translate(glm::vec3(x, y, z));
}

void Camera::rotateUp(float angle )
{
	m_forward = glm::angleAxis(angle, m_right) * m_forward;
	m_viewMatrix = glm::lookAt(
		m_position,
		m_position + m_forward,
		m_up);
}

void Camera::rotateRight(float angle)
{
	m_forward = glm::angleAxis(-angle, m_up) * m_forward;
	m_right = glm::cross(m_forward, m_up);
	m_viewMatrix = glm::lookAt(
		m_position,
		m_position + m_forward,
		m_up);
}

const glm::mat4 Camera::ProjectionMatrix() const
{
	return m_projectionMatrix;
}

const glm::mat4 Camera::ViewMatrix() const
{
	return m_viewMatrix;
}

glm::vec3 Camera::Position() const {return m_position;}
glm::vec3 Camera::Up() const { return m_up; }
glm::vec3 Camera::Forward() const { return m_forward; }
glm::vec3 Camera::Right() const { return m_right; }