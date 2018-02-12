#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Shader.h"
#include "Physics.h"
#include "Texture.h"
#include "Camera.h"
#include "GameObject.h"
#include "PlayerController.h"
#include "Input.h"
#include "Mesh.h"

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