#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
 
#include <iostream>
#include <string>
 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
#include "graphics/Text.h"
#include "graphics/FrameBuffer.h"
#include "graphics/CubeMap.h"
  
   
#include "util/Debug.h"
#include "util/Time.h"
#include "util/Input.h" 
#include "util/Statistics.h" 
    
//Design pattern singleton   
class Minecraft 
{  
public:
	Minecraft(std::string name, int width, int height);
	~Minecraft();
	 
	enum PlayerState{ spectator, survival };
	
	void Start( ); 
	   
private:



	void SetupPostProcess();
	void SetupSkyBox();

	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_samples;

	static GLFWwindow* m_window;

	unsigned int postProcVAO, postProcVBO;
	unsigned int skyboxVAO, skyboxVBO;
	 
	PlayerState m_playerState = PlayerState::survival;
};   