#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "btBulletDynamicsCommon.h"

#include "util/Debug.h"
#include "util/MoreMath.h"



class Camera : IWithDebug
{
public:
	Camera( int width, int height, float farPlane = 100.f, float nearPlane = 0.1f, float fov = 60.f);

	void SetPosition(glm::vec3 vector);
	void Translate(glm::vec3 vector);
	void RotateUp(float angle);
	void RotateRight(float angle);

	const glm::mat4 projectionMatrix() const;
	const glm::mat4 viewMatrix() const;

	glm::vec3 position() const ;
	glm::vec3 Up() const;
	glm::vec3 UpWorld() const;
	glm::vec3 forward() const;
	glm::vec3 right() const;

	glm::vec2 Size()const;
	float Far() const;
	float Near() const;
	float Fov()const;

	void OnDrawDebug() const override;

	bool InsideFrustrum(glm::vec3 point) const;
	std::vector<Plane> GetFrustrumPlanes() const;


private:

	glm::vec3 m_position;
	glm::vec3 m_up;
	glm::vec3 m_upWorld;
	glm::vec3 m_forward;
	glm::vec3 m_right;
	
	glm::mat4 m_projectionMatrix;

	mutable glm::mat4 m_viewMatrix;
	mutable bool viewMatrixChanged = false;
	
	glm::vec2 m_size;
	float m_far;
	float m_near;
	float m_fov;
};