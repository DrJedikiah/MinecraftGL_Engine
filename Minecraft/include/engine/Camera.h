#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera( int width, int height, float far = 100.f, float near = 0.1f, float fov = 45.f);

	void translate(glm::vec3 vector);
	void translate (float x, float y, float z );
	void rotateUp(float angle);
	void rotateRight(float angle);

	const glm::mat4 ProjectionMatrix() const;
	const glm::mat4 ViewMatrix() const;

	glm::vec3 Position() const ;
	glm::vec3 Up() const;
	glm::vec3 Forward() const;
	glm::vec3 Right() const;

private:
	glm::vec3 m_position;
	glm::vec3 m_up;
	glm::vec3 m_forward;
	glm::vec3 m_right;
	
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
};