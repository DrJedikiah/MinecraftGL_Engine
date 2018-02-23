#include "util/Input.h"

////////////Input////////////
unsigned Input::m_count = 0;

void Input::Setup(GLFWwindow * window)
{	
	m_window = window;	
	glfwGetWindowSize(window, &(m_windowSize.x), &(m_windowSize.y));

	double x, y;
	glfwGetCursorPos(m_window, &x, &y);
	Input::Update();

	glfwSetFramebufferSizeCallback(m_window, Input::window_size_callback);
	glfwSetCursorPosCallback(m_window, Mouse::mouse_callback);
	glfwSetKeyCallback(m_window, Keyboard::key_callback);
	glfwSetMouseButtonCallback(m_window, Mouse::mouse_button_callback);
}

void Input::window_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLFWwindow * Input::GetWindow()	{return m_window;}
glm::ivec2 Input::GetWindowSize(){ return m_windowSize; }

GLFWwindow * Input::m_window = nullptr;
glm::ivec2  Input::m_windowSize;

unsigned Input::FrameCount() { return m_count;  }

bool Input::ShuttingDown()
{
	return glfwWindowShouldClose(m_window);
}

void Input::Update()
{
	++m_count;
	Mouse::Update(m_count);
	glfwPollEvents();
}

////////////Keyboard////////////
std::array< unsigned, 349 > Keyboard::m_keysPressed;
std::array< unsigned, 349 > Keyboard::m_keysReleased;


int Keyboard::KeyDown(int GLFW_KEY){return glfwGetKey(Input::GetWindow(), GLFW_KEY) == GLFW_PRESS;}

bool Keyboard::KeyPressed(int GLFW_KEY) { return m_keysPressed[GLFW_KEY] == Input::FrameCount();}
bool Keyboard::KeyReleased(int GLFW_KEY) { return m_keysReleased[GLFW_KEY] == Input::FrameCount(); }
void Keyboard::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		m_keysPressed[key] = Input::FrameCount();
	else if (action == GLFW_RELEASE)
		m_keysReleased[key] = Input::FrameCount();
}

////////////Mouse////////////

glm::vec2 Mouse::position() { return m_position; }
glm::vec2 Mouse::delta() { return m_delta; }
std::array< unsigned, 11 > Mouse::m_buttonsPressed;
std::array< unsigned, 11 > Mouse::m_buttonsReleased;


void Mouse::SetCursor(CursorState state)
{
		glfwSetInputMode(Input::GetWindow(), GLFW_CURSOR, state);
}

void Mouse::CenterCursor(bool state)
{
	m_centerCursor = state;
	glm::ivec2 size = Input::GetWindowSize();
	glfwSetCursorPos(Input::GetWindow(), size.x / 2, size.y / 2);
}

bool Mouse::KeyDown(int  GLFW_MOUSE_BUTTON)
{
	return glfwGetMouseButton(Input::GetWindow(), GLFW_MOUSE_BUTTON) == GLFW_PRESS;
}

bool Mouse::ButtonPressed(int GLFW_MOUSE_BUTTON) { return m_buttonsPressed[GLFW_MOUSE_BUTTON] == Input::FrameCount(); }
bool Mouse::ButtonReleased(int GLFW_MOUSE_BUTTON) { return m_buttonsReleased[GLFW_MOUSE_BUTTON] == Input::FrameCount(); }

void Mouse::mouse_callback(GLFWwindow* window, double x, double y)
{
	
}

void Mouse::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
		m_buttonsPressed[button] = Input::FrameCount();
	else if (action == GLFW_RELEASE)
		m_buttonsReleased[button] = Input::FrameCount();
}

void Mouse::Update(int count)
{
	//Mouse
	double x, y;
	glfwGetCursorPos( Input::GetWindow(), &x, &y);

	if (m_centerCursor)
	{
		GLFWwindow * window = Input::GetWindow();
		glm::ivec2 size = Input::GetWindowSize();
		glfwSetCursorPos(window, size.x / 2, size.y / 2);

		m_position = glm::vec2(x, y);
		m_delta = m_position - glm::vec2(size.x/2, size.y/2 );
	}
	else
	{
		m_oldPosition = m_position;
		m_position = glm::vec2(x, y);
		m_delta = m_position - m_oldPosition;
	}
}

bool Mouse::m_centerCursor = false;
glm::vec2 Mouse::m_oldPosition;
glm::vec2 Mouse::m_position;
glm::vec2 Mouse::m_delta;