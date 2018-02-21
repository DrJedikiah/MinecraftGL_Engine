#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <array>

class Input
{
	friend class Minecraft;

public:
	static void Setup(GLFWwindow * window);
	static GLFWwindow * GetWindow();
	static glm::ivec2 GetWindowSize();
	static unsigned GetCount();

private:
	static void window_size_callback(GLFWwindow* window, int width, int height);
	static GLFWwindow * m_window;
	static glm::ivec2 m_windowSize;

	static void Update();

	static unsigned m_count;
};


class Keyboard
{
	friend class Input;

public:
	static int KeyDown(int GLFW_KEY);
	static bool KeyPressed(int GLFW_KEY);
	static bool KeyReleased(int GLFW_KEY);


	enum AzertyKey {
		D = GLFW_KEY_D,
		Q = GLFW_KEY_A,
		S = GLFW_KEY_S,
		Z = GLFW_KEY_W,
		A = GLFW_KEY_Q,
		E = GLFW_KEY_E
	};

private:
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	//Events
	static std::array< unsigned, 349 > m_keysPressed;
	static std::array< unsigned, 349 > m_keysReleased;

};

class Mouse
{
	friend class Input;

public:
	enum CursorState { disabled = GLFW_CURSOR_DISABLED, hidden = GLFW_CURSOR_HIDDEN, normal = GLFW_CURSOR_NORMAL };
	
	static glm::vec2 position();
	static glm::vec2 delta();

	static void SetCursor(CursorState state );
	static void CenterCursor( bool state);

	static bool KeyDown(int GLFW_MOUSE_BUTTON);

private:
	static void mouse_callback(GLFWwindow* window, double x, double y);
	static void Update(int count);

	static bool m_centerCursor;

	static glm::vec2 m_oldPosition;
	static glm::vec2 m_position;
	static glm::vec2 m_delta;

};



