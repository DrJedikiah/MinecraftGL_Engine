#include "graphics/Mesh.h"

Mesh::Mesh()
{
	m_empty = true;
}

Mesh::Mesh(std::vector<Vertex> verticesData)
{
	if (verticesData.size() > 0)
	{
		m_empty = false;
		m_size = verticesData.size();

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(Vertex), verticesData.data(), GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);

		// texture coord attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, normals)));
		glEnableVertexAttribArray(1);

		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, texCoord)));
		glEnableVertexAttribArray(2);
	}
	else
		m_empty = true;
	
}

void Mesh::Draw(const Shader& shader) const
{
	if (! m_empty)
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, m_size);
	}
}

Mesh::~Mesh()
{
	if (!m_empty)
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
}