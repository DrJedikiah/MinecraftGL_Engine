#include "Minecraft.h"

Minecraft::Minecraft(std::string name, int width, int height)
{
	m_scr_width = width;
	m_scr_height = height;

	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
	#endif

	// glfw window creation
	m_window = glfwCreateWindow(m_scr_width, m_scr_height, name.c_str(), NULL, NULL);
	if (m_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_window);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//Callbacks
	glfwSetFramebufferSizeCallback(m_window, Minecraft::window_size_callback);
	glfwSetCursorPosCallback(m_window, mouse_callback);

	//Initialisation
	Input::SetWindow(m_window);
	{
		double x, y;
		glfwGetCursorPos(m_window, &x, &y);
		Mouse::Update();
	}


	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	//Setup openGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Minecraft::Start()
{

	std::vector<Vertex> vertices = 
	{
		//Back
		{ {-0.5f, -0.5f, -0.5f},	{ 0.f,0.f,-1.f},	{ 0.f, 0.5f } },//bot
		{ { 0.5f,  0.5f, -0.5f },	{ 0.f,0.f,-1.f },	{ 0.5f, 1.0f } },
		{ { 0.5f, -0.5f, -0.5f },	{ 0.f,0.f,-1.f },	{ 0.5f, 0.5f } },
		{ { 0.5f,  0.5f, -0.5f },	{ 0.f,0.f,-1.f },	{ 0.5f, 1.0f } },//Top
		{ { -0.5f, -0.5f, -0.5f },	{ 0.f,0.f,-1.f },	{ 0.f, 0.5f } },
		{ { -0.5f,  0.5f, -0.5f },	{ 0.f,0.f,-1.f },	{ 0.f, 1.0f } },

		//Front
		{ { -0.5f, -0.5f, 0.5f },	{ 0.f,0.f,1.f },	{ 0.f, 0.5f } },//bot
		{ { 0.5f, -0.5f, 0.5f },	{ 0.f,0.f,1.f },	{ 0.5f, 0.5f } },
		{ { 0.5f,  0.5f, 0.5f },	{ 0.f,0.f,1.f },	{ 0.5f, 1.0f } },
		{ { 0.5f,  0.5f, 0.5f },	{ 0.f,0.f,1.f },	{ 0.5f, 1.0f } },//Top
		{ { -0.5f,  0.5f, 0.5f },	{ 0.f,0.f,1.f },	{ 0.f, 1.0f } },
		{ { -0.5f, -0.5f, 0.5f },	{ 0.f,0.f,1.f },	{ 0.f, 0.5f } },
		
		//Left
		{ { -0.5f,  0.5f,  0.5f },{ -1.f,0.f,0.f },{ 0.f, 1.f } },//Top
		{ { -0.5f,  0.5f, -0.5f },{ -1.f,0.f,0.f },{ 0.5f, 1.f } },
		{ { -0.5f, -0.5f, -0.5f },{ -1.f,0.f,0.f },{ 0.5f, 0.5f } },
		{ { -0.5f, -0.5f, -0.5f },{ -1.f,0.f,0.f },{ 0.5f, 0.5f } },//Bot
		{ { -0.5f, -0.5f,  0.5f },{ -1.f,0.f,0.f },{ 0.f, 0.5f } },
		{ { -0.5f,  0.5f,  0.5f },{ -1.f,0.f,0.f },{ 0.f, 1.f } },

		//Right
		{ { 0.5f,  0.5f,  0.5f },{ 1.f,0.f,0.f },{ 0.f, 1.f } },//Top
		{ { 0.5f, -0.5f, -0.5f },{ 1.f,0.f,0.f },{ 0.5f, 0.5f } },
		{ { 0.5f,  0.5f, -0.5f },{ 1.f,0.f,0.f },{ 0.5f, 1.f } },
		{ { 0.5f, -0.5f, -0.5f },{ 1.f,0.f,0.f },{ 0.5f, 0.5f } },//Bot
		{ { 0.5f,  0.5f,  0.5f },{ 1.f,0.f,0.f },{ 0.f, 1.f } },
		{ { 0.5f, -0.5f,  0.5f },{ 1.f,0.f,0.f },{ 0.f, 0.5f } },

		//Bot
		{ { -0.5f, -0.5f, -0.5f },	{ 0.f,-1.f,0.f },{ 0.5f, 1.0f } },
		{ { 0.5f, -0.5f, -0.5f },	{ 0.f,-1.f,0.f },{ 1.0f, 1.0f } },
		{ { 0.5f, -0.5f,  0.5f },	{ 0.f,-1.f,0.f },{ 1.0f, 0.5f } },
		{ { 0.5f, -0.5f,  0.5f },	{ 0.f,-1.f,0.f },{ 1.0f, 0.5f } },
		{ { -0.5f, -0.5f,  0.5f },	{ 0.f,-1.f,0.f },{ 0.5f, 0.5f } },
		{ { -0.5f, -0.5f, -0.5f },	{ 0.f,-1.f,0.f },{ 0.5f, 1.0f } },

		//Top
		{ { 0.5f,  0.5f, -0.5f },	{ 0.f,1.f,0.f },{ 0.5f, 0.5f}},
		{ { -0.5f,  0.5f, -0.5f },	{ 0.f,1.f,0.f },{ 0.0f, 0.5f } },
		{ { 0.5f,  0.5f,  0.5f },	{ 0.f,1.f,0.f },{ 0.5f, 0.0f } },
		{ { -0.5f,  0.5f,  0.5f },	{ 0.f,1.f,0.f },{ 0.0f, 0.0f } },
		{ { 0.5f,  0.5f,  0.5f },	{ 0.f,1.f,0.f },{ 0.5f, 0.0f } },
		{ { -0.5f,  0.5f, -0.5f },	{ 0.f,1.f,0.f },{ 0.0f, 0.5f } }
	};


	//Shader
	Shader shader("shaders/shader.vs", "shaders/shader.fs");
	Texture texture("textures/grass.bmp");


	std::vector<glm::vec3> lights =
	{
		{ 0, 0, 0 },
		{ -1,3, 3 }
	};

	Mesh cubeMesh(vertices, texture);
	Model cube({ cubeMesh });
		cube.Translate({ 5, 0, 0 });
	Model sun1({ cubeMesh });
		sun1.SetPosition(lights[0]);
	Model sun2({ cubeMesh });
		sun2.SetPosition(lights[1]);

	Camera camera(m_scr_width, m_scr_height);
	PlayerController playerController(camera);

	shader.setFloat("ambient", 0.2f);
	shader.setMat4("view", camera.ViewMatrix());
	shader.setMat4("projection", camera.ProjectionMatrix());
	shader.setInt("numLights", lights.size());

	float time = (float)glfwGetTime();

	float drawTimer = 0.f;
	float drawDelta = 1.f / 60.f;

	float fixedUpdateTimer = 0.f;
	float fixedUpdateDelta = 0.02f;

	// render loop
	while (!glfwWindowShouldClose(m_window))
	{
		float delta = (float)glfwGetTime() - time;
		time += delta;

		//Update
		glfwPollEvents();
		if( Keyboard::IsKeyDown(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(m_window, true);
		
		//Fixed update
		fixedUpdateTimer += delta;
		if (fixedUpdateTimer >= fixedUpdateDelta)
		{
			Mouse::Update();
			fixedUpdateTimer = 0.f;
			playerController.Update(fixedUpdateDelta);

			cube.RotateAround(fixedUpdateDelta, { 0,0,0 }, { 0,0,1 });
		}

		//Draws
		drawTimer += delta;
		if (drawTimer >= drawDelta)
		{
			drawTimer = 0.f;

			//Uniforms
			shader.setMat4("view", camera.ViewMatrix());
			shader.setVec3Array("lights", lights);
			shader.setVec3("viewPos", camera.Position());

			//Clear
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			//Draw
			sun1.Draw(shader);
			sun2.Draw(shader);
			cube.Draw(shader);
			glfwSwapBuffers(m_window);
		}
	}

	glfwTerminate();
}

void Minecraft::window_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Minecraft::mouse_callback(GLFWwindow* window, double x, double y)
{
	
}

Minecraft::~Minecraft()
{

}