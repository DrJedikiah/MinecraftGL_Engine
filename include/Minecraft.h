#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "graphics/Shader.h"
#include "engine/Physics.h"
#include "graphics/Texture.h"
#include "engine/Camera.h"
#include "graphics/Model.h"
#include "engine/PlayerController.h"
#include "util/Input.h"
#include "graphics/Mesh.h"
#include "graphics/Light.h"
#include "engine/World.h"
#include "util/Time.h"

//Design pattern singleton
class Minecraft
{
public:
	Minecraft(std::string name, int width, int height);
	~Minecraft();
	
	void Start();

private:
	unsigned int m_scr_width;
	unsigned int m_scr_height;
	GLFWwindow* m_window;

	static void window_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double x, double y);
};