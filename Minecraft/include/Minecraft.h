#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "engine/Physics.h"
#include "engine/Camera.h"
#include "engine/PlayerController.h"
#include "engine/FreeCameraController.h"
#include "engine/World.h"
#include "engine/Cube.h"
#include "engine/PlayerAvatar.h"

#include "graphics/Shader.h"
#include "graphics/TexturesBlocks.h"
#include "graphics/Mesh.h"
#include "graphics/Light.h"
#include "graphics/Texture.h"
#include "graphics/Model.h"

#include "util/Time.h"
#include "util/Input.h"

//Design pattern singleton
class Minecraft
{
public:
	Minecraft(std::string name, int width, int height);
	~Minecraft();

	enum PlayerState{ spectator, survival };
	
	void Start();
	World world;

private:
	unsigned int m_scr_width;
	unsigned int m_scr_height;
	GLFWwindow* m_window;

	PlayerState m_playerState = PlayerState::survival;
};