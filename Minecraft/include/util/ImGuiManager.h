#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#ifdef _WIN32
#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#endif

struct GLFWwindow;

class ImGuiManager
{
public:


	static bool        Init(GLFWwindow* window);
	static void        Shutdown();
	static void        NewFrame();

	// Use if you want to reset your rendering device without losing ImGui state.
	static void        InvalidateDeviceObjects();
	static bool        CreateDeviceObjects();

	// GLFW callbacks (installed by default if you enable 'install_callbacks' during initialization)
	// Provided here if you want to chain callbacks.
	// You can also handle inputs yourself and use those as a reference.
	static void        MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void        ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void        KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void        CharCallback(GLFWwindow* window, unsigned int c);

private:
	// This is the main rendering function that you have to implement and provide to ImGui (via setting up 'RenderDrawListsFn' in the ImGuiIO structure)
	// If text or lines are blurry when integrating ImGui in your engine:
	// - in your Render function, try translating your projection matrix by (0.5f,0.5f) or (0.375f,0.375f)
	static void RenderDrawLists(ImDrawData* draw_data);
	static const char* GetClipboardText(void* user_data);
	static void SetClipboardText(void* user_data, const char* text);
	static bool CreateFontsTexture();

	static GLFWwindow*  g_Window;
	static double       g_Time;
	static bool         g_MousePressed[3];
	static float        g_MouseWheel;
	static GLuint       g_FontTexture;
	static int          g_ShaderHandle, g_VertHandle, g_FragHandle;
	static int          g_AttribLocationTex, g_AttribLocationProjMtx;
	static int          g_AttribLocationPosition, g_AttribLocationUV, g_AttribLocationColor;
	static unsigned int g_VboHandle, g_VaoHandle, g_ElementsHandle;
};
