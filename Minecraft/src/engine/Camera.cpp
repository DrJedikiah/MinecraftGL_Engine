#include "engine/Camera.h"

Camera::Camera(int width, int height, float farPlane, float nearPlane, float fov) :
	m_projectionMatrix(glm::perspective(glm::radians(fov), (float)width / (float)height, nearPlane, farPlane)),
	m_position(0,0,0),
	m_upWorld(0,1,0),
	m_forward(1, 0, 0),
	m_right( glm::cross(m_forward, m_upWorld)),
	m_up(m_upWorld),
	m_viewMatrix(glm::lookAt(m_position, m_position + m_forward, m_upWorld)),
	m_size(width, height),
	m_far(farPlane),
	m_near(nearPlane),
	m_fov(fov)
{	
	float farHeight = m_far * tan(glm::radians(m_fov / 2));
	float farWidth = (m_size.x / m_size.y) * farHeight;
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
	m_up = glm::cross(m_right, m_forward);
	viewMatrixChanged = true;

}

void Camera::RotateRight(float angle)
{
	m_forward = glm::angleAxis(-angle, m_upWorld) * m_forward;
	m_right = glm::normalize(glm::cross(m_forward, m_upWorld));
	m_up = glm::cross(m_right, m_forward);
	viewMatrixChanged = true;
}


const glm::mat4 Camera::projectionMatrix() const { return m_projectionMatrix; }
glm::vec3 Camera::position() const {return m_position;}


glm::vec3 Camera::Up() const { return m_up; }
glm::vec3 Camera::UpWorld() const { return m_upWorld; }

glm::vec3 Camera::forward() const { return m_forward; }
glm::vec3 Camera::right() const { return m_right; }

const glm::mat4 Camera::viewMatrix() const
{
	if (viewMatrixChanged)
	{
		viewMatrixChanged = false;
		m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_upWorld);
	}
	return m_viewMatrix;
}

glm::vec2 Camera::Size()const { return m_size; }
float Camera::Far() const { return m_far; }
float Camera::Near() const { return m_near; }
float Camera::Fov()const { return m_fov; }

void Camera::OnDrawDebug() const
{

	//Draw the camera frustrum
	glm::vec4 red(1, 0, 0, 0.5f);
	
	float farHeight =  m_far * tan( glm::radians(m_fov/2));
	float farWidth = (m_size.x/ m_size.y) * farHeight;

	glm::vec3 farCenter = m_position + m_far * m_forward;
	glm::vec3 farTR = farCenter + farHeight * m_up + farWidth * m_right;
	glm::vec3 farTL = farCenter + farHeight * m_up - farWidth * m_right;
	glm::vec3 farBR = farCenter - farHeight * m_up + farWidth * m_right;
	glm::vec3 farBL = farCenter - farHeight * m_up - farWidth * m_right;

	Debug::DrawLine(m_position, farTR, red);
	Debug::DrawLine(m_position, farTL, red);
	Debug::DrawLine(m_position, farBR, red);
	Debug::DrawLine(m_position, farBL, red);
	Debug::DrawLine(farTR, farTL, red);
	Debug::DrawLine(farBR, farBL, red);
	Debug::DrawLine(farTR, farBR, red);
	Debug::DrawLine(farTL, farBL, red);

}

bool Camera::InsideFrustrum(glm::vec3 point) const
{
	float farHeight = m_far * tan(glm::radians(m_fov / 2));
	float farWidth = (m_size.x / m_size.y) * farHeight;
	glm::vec3 farCenter = m_position + m_far * m_forward;
	glm::vec3 TR = farCenter + farHeight * m_up + farWidth * m_right;
	glm::vec3 TL = farCenter + farHeight * m_up - farWidth * m_right;
	glm::vec3 BR = farCenter - farHeight * m_up + farWidth * m_right;
	glm::vec3 BL = farCenter - farHeight * m_up - farWidth * m_right;

	Plane rightPlane = Plane( m_position, glm::cross(TR - BR, m_position - BR));
	Plane leftPlane = Plane(m_position, glm::cross(m_position - BL, TL - BL));
	Plane topPlane = Plane(m_position, glm::cross(TL - TR, m_position - TL));
	Plane botPlane = Plane(m_position, glm::cross(m_position - BL, BL - BR));

	return  !rightPlane.Right(point) && !leftPlane.Right(point) && !topPlane.Right(point) && !botPlane.Right(point);
		
}

std::vector<Plane> Camera::GetFrustrumPlanes() const
{
	float farHeight = m_far * tan(glm::radians(m_fov / 2));
	float farWidth = (m_size.x / m_size.y) * farHeight;
	glm::vec3 farCenter = m_position + m_far * m_forward;
	glm::vec3 TR = farCenter + farHeight * m_up + farWidth * m_right;
	glm::vec3 TL = farCenter + farHeight * m_up - farWidth * m_right;
	glm::vec3 BR = farCenter - farHeight * m_up + farWidth * m_right;
	glm::vec3 BL = farCenter - farHeight * m_up - farWidth * m_right;

	Plane rightPlane = Plane(m_position, glm::cross(TR - BR, m_position - BR));
	Plane leftPlane = Plane(m_position, glm::cross(m_position - BL, TL - BL));
	Plane topPlane = Plane(m_position, glm::cross(TL - TR, m_position - TL));
	Plane botPlane = Plane(m_position, glm::cross(m_position - BL, BL - BR));

	return { rightPlane, leftPlane, topPlane, botPlane };
}



