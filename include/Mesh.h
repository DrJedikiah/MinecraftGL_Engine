#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "Texture.h"
#include "Shader.h"

struct Vertex 
{
	glm::vec3 pos;
	glm::vec2 texCoord;
};

class Mesh {
public:
	/*  Mesh Data  */
	std::vector<Vertex> vertices;
	Texture & texture;

	/*  Functions  */
	Mesh(std::vector<Vertex> vertices, Texture& texture);
	virtual void Draw(Shader shader);

private:
	unsigned int VAO, VBO;

	void setupMesh();
};