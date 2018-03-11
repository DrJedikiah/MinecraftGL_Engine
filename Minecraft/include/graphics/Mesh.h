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


	Texture * texture;
	Mesh(std::vector<Vertex> vertices);
	~Mesh();

	bool Empty() const;

	void Draw(const Shader& shader) const override;

private:
	unsigned int VAO, VBO;
	unsigned int m_size;
	bool m_empty = true;
};

