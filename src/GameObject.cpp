#include "GameObject.h"

GameObject::GameObject() : m_modelMatrix(1.f)
{

}

void GameObject::translate(glm::vec3 vector)
{
	m_modelMatrix = glm::translate(m_modelMatrix, vector);
}

const glm::mat4 GameObject::ModelMatrix() const
{
	return m_modelMatrix;
}