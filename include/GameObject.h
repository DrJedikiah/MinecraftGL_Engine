#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class GameObject
{
public:
	GameObject();

	const glm::mat4 ModelMatrix() const;

	void translate(glm::vec3 vector);

private:

	glm::mat4 m_modelMatrix;
};