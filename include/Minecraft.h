#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Shader.h"
#include "Physics.h"
#include "Texture.h"

//Design pattern singleton
class Minecraft
{
public:
	~Minecraft();
	void Init( std::string name, int width, int height);
	void Start();
	static Minecraft& Instance();

private:
	Minecraft();
	Minecraft & operator= (const Minecraft&) {}
	Minecraft(const Minecraft&) {}
	

	static Minecraft m_instance;
	unsigned int m_scr_width;
	unsigned int m_scr_height;
	GLFWwindow* m_window;

	static void size_callback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow *window);
};