#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "graphics/Mesh.h"

class Model
{
public:
	Model( std::vector<Mesh> meshList );

	void Draw( Shader shader) const;

	const glm::mat4 ModelMatrix() const;

	void SetPosition(glm::vec3 vector);
	void Translate(glm::vec3 vector);
	void SetRotation(glm::vec3 eulerAngles);
	void Rotate(float angle, glm::vec3 axis);
	void RotateAround(float angle, glm::vec3 point, glm::vec3 axis);

	glm::vec3 Position();
	glm::vec3 Rotation();

private:
	mutable bool modelMatrixChanged = true;
	void UpdateModelMatrix() const;
	mutable glm::mat4 m_modelMatrix;

	glm::vec3 m_position;
	glm::quat m_rotation;

	std::vector<Mesh> m_meshList;
	
};