#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "graphics/Texture.h"
#include "graphics/Shader.h"
#include "graphics/Drawable.h"

class Mesh  : public Drawable
{
public:
	struct Vertex
	{
		glm::vec3 vertex;
		glm::vec3 normals;
		glm::vec2 texCoord;
	};

	/*  Mesh Data  */
	//std::vector<Vertex> vertices;
	Texture * texture;
	Mesh(std::vector<Vertex> vertices);

	void Draw(const Shader& shader) const override;

private:
	unsigned int VAO, VBO;
	unsigned int m_size;

	void setupMesh(std::vector<Vertex> vertices);
};

