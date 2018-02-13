#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "Texture.h"
#include "Shader.h"

struct Vertex 
{
	glm::vec3 vertex;
	glm::vec3 normals;
	glm::vec2 texCoord;
};

class Mesh {
public:
	/*  Mesh Data  */
	std::vector<Vertex> vertices;
	Texture * texture;
	Mesh(std::vector<Vertex> vertices);

	virtual void Draw(const Shader shader) const;

private:
	unsigned int VAO, VBO;

	void setupMesh();
};

