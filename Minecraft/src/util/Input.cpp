#include "util/Input.h"

////////////Input////////////
void Input::SetWindow(GLFWwindow * window)
{
	m_window = window;
}

GLFWwindow * Input::m_window = nullptr;

////////////Keyboard////////////

int Keyboard::IsKeyDown(int GLFW_KEY)
{
	return glfwGetKey(Input::m_window, GLFW_KEY) == GLFW_PRESS;
}

////////////Mouse////////////

glm::vec2 Mouse::Position() { return m_position; }
glm::vec2 Mouse::Delta() { return m_delta; }
bool Mouse::KeyDown(int  GLFW_MOUSE_BUTTON)
{
	return glfwGetMouseButton(Input::m_window, GLFW_MOUSE_BUTTON) == GLFW_PRESS;
}

void Mouse::Update()
{
	//Mouse
	double x, y;
	glfwGetCursorPos( Input::m_window, &x, &y);

	m_oldPosition = m_position;
	m_position = glm::vec2(x, y);
	m_delta = m_position - m_oldPosition;
}


glm::vec2 Mouse::m_oldPosition;
glm::vec2 Mouse::m_position;
glm::vec2 Mouse::m_delta;