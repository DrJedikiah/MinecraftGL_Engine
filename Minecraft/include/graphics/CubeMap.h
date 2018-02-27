#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>

class CubeMap
{
public:
	CubeMap( std::vector<std::string> filenames);
	unsigned int textureID;


};