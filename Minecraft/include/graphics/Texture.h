#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <iostream>

class Texture
{

public:
	Texture(std::string filename);
	void Use() const;

	unsigned int textureId;
private:
	int m_width, m_height, m_nrChannels;
};

