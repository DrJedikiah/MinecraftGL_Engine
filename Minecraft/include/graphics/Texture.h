#pragma once

#include <iostream>
#include <vector>


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

enum TextureUnit;

class Texture
{

public:
	Texture(std::string filename);
	Texture(int width, int height, std::vector<glm::vec3> texels);

	void Use( TextureUnit textureUnit ) const;

private:
	unsigned int textureId;
	int m_width, m_height, m_nrChannels;
};

enum TextureUnit
{
	Unit0 = GL_TEXTURE0,
	Unit1 = GL_TEXTURE1,
	Unit2 = GL_TEXTURE2,
	Unit3 = GL_TEXTURE3,
	Unit4 = GL_TEXTURE4,
	Unit5 = GL_TEXTURE5,
	Unit6 = GL_TEXTURE6,
	Unit7 = GL_TEXTURE7,
	Unit8 = GL_TEXTURE8,
	Unit9 = GL_TEXTURE9,
	Unit10 = GL_TEXTURE10,
	Unit11 = GL_TEXTURE11
};