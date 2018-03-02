#pragma once

#include <glm/glm.hpp>
#include "graphics/Shader.h"

class Light
{
public:
	Light(
		glm::vec3 Position,
		glm::vec3 Ambient = glm::vec3(1.f, 1.f, 1.f),
		glm::vec3 Diffuse = glm::vec3(1.f, 1.f, 1.f),
		glm::vec3 Specular = glm::vec3(1.f, 1.f, 1.f)
	);

	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};