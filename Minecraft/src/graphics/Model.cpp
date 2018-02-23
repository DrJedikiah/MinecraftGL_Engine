#include "graphics/Model.h"



Model::Model(std::vector<Mesh::Vertex> vertices) :
	m_modelMatrix(1.f),
	m_position(0.f, 0.f, 0.f),
	m_rotation(glm::vec3(0, 0, 0)),
	m_mesh(vertices)
{

}

Model::Model()
{

}

void Model::SetPosition(glm::vec3 vector)
{
	m_position = vector;
	modelMatrixChanged = true;
}

void Model::Translate(glm::vec3 vector)
{
	m_position += vector;
	modelMatrixChanged = true;
}

void Model::SetRotation( glm::vec3 eulerAngles)
{
	m_rotation = glm::quat(eulerAngles);
	modelMatrixChanged = true;
}

void Model::Rotate( float angle, glm::vec3 axis)
{
	m_rotation = glm::rotate(m_rotation, angle, axis);
	modelMatrixChanged = true;
}

void Model::RotateAround(float angle, glm::vec3 point, glm::vec3 axis)
{
	m_position = glm::quat(angle * axis) * (m_position - point ) + point;
	m_rotation = glm::rotate(m_rotation, angle, axis);
	modelMatrixChanged = true;
}

void Model::UpdateModelMatrix() const
{
	m_modelMatrix = glm::translate(glm::mat4(1.f), m_position) * glm::mat4_cast(m_rotation);
	modelMatrixChanged = false;
}

glm::vec3 Model::Position() { return m_position; }
glm::vec3 Model::Rotation() { return glm::eulerAngles(m_rotation); }

const glm::mat4 Model::ModelMatrix() const
{
	UpdateModelMatrix();
	return m_modelMatrix;
}

void Model::Draw(const Shader& shader) const
{
	if (modelMatrixChanged)
		UpdateModelMatrix();
		
	shader.setMat4("model", m_modelMatrix);
	m_mesh.Draw(shader);
}

Model::~Model()
{
}