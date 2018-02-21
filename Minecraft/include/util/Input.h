#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>

class Input
{
public:
	friend class Keyboard;
	friend class Mouse;

	static void SetWindow(GLFWwindow * window);
	

private:
	static GLFWwindow * m_window;
};


class Keyboard
{
public:


	static int IsKeyDown(int GLFW_KEY);


};

class Mouse
{
public:

	static glm::vec2 Position();
	static glm::vec2 Delta();
	static bool KeyDown(int GLFW_MOUSE_BUTTON);
	static void Update();

private:
	static glm::vec2 m_oldPosition;
	static glm::vec2 m_position;
	static glm::vec2 m_delta;

};



enum AzertyKey {
	D = GLFW_KEY_D,
	Q = GLFW_KEY_A,
	S = GLFW_KEY_S,
	Z = GLFW_KEY_W,
	A = GLFW_KEY_Q,
	E = GLFW_KEY_E
};