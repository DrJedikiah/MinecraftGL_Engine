#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>

#include "graphics/Texture.h"

class CubeMap
{
public:
	CubeMap( std::vector<std::string> filenames);
	void UseTexture(TextureUnit textureUnit)const;

private:
	unsigned int textureID;


};