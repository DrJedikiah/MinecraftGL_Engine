#include "Minecraft.h"

GLFWwindow* Minecraft::m_window = nullptr;

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

	Input::Setup(m_window);

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
	float t1 = Time::ElapsedSinceStartup();
	//Shader
	Shader shader("shaders/shader.vs", "shaders/shader.fs");
	Texture texture("textures/grass.bmp");

	//Block Tiles and textures
	Tiles::Initialize(2,2);
	TexturesBlocks::Initialize();

	//Lights
	std::vector<Light> lights =
	{
		Light({ 30,30,14.5 }),
	};

	shader.use();
	texture.Use();

	World::GenerateChunks();
	World::GeneratePhysics();

	Camera camera(m_scr_width, m_scr_height, 1000);
	camera.Translate(glm::vec3(14, 16, 14 + 8) - camera.position());
	camera.RotateUp(glm::radians(-15.f));

	PlayerAvatar player;
	player.rb().translate(btVector3(8, 25, 8));

	FreeCameraController freeCameraController( camera);
	freeCameraController.SetEnabled(false);
	PlayerController playerController(camera, player);
	playerController.SetEnabled(true);

	Cube cube;
	cube.rb().translate(btVector3(4.5, 20, 4.5));

	shader.setFloat("ambient", 0.2f);
	shader.setMat4("view", camera.viewMatrix());
	shader.setMat4("projection", camera.projectionMatrix());
	shader.setInt("numLights", lights.size());

	float drawTimer = 0.f;
	float fixedUpdateTimer = 0.f;
	
	float time = Time::ElapsedSinceStartup();

	float t2 = Time::ElapsedSinceStartup();
	std::cout << 1000.f*(t2 - t1) << std::endl;

	// render loop
	while (!glfwWindowShouldClose(m_window))
	{
		float delta = Time::ElapsedSinceStartup() - time;
		time += delta;

		//Fixed update
		fixedUpdateTimer += delta;
		if (fixedUpdateTimer >= Time::FixedDeltaTime())
		{
			fixedUpdateTimer = 0.f;

			Input::Update();

			//Toogle controllers
			if (Keyboard::KeyPressed(GLFW_KEY_F1))
			{
				freeCameraController.SetEnabled( ! freeCameraController.Enabled() );
				playerController.SetEnabled( ! playerController.Enabled() );
			}
			if (Keyboard::KeyPressed(GLFW_KEY_ESCAPE))
				glfwSetWindowShouldClose(m_window, true);
				

			PhysicsEngine::StepSimulation(Time::FixedDeltaTime());

			freeCameraController.Update(Time::FixedDeltaTime());
			playerController.Update(Time::FixedDeltaTime());

			player.Update(Time::FixedDeltaTime());
			cube.Update(Time::FixedDeltaTime());
		}

		//Draws
		drawTimer += delta;
		if (drawTimer >= Time::DeltaTime())
		{
			drawTimer = 0.f;
			//Uniforms
			shader.setMat4("view", camera.viewMatrix());
			for (int i = 0; i < (int)lights.size(); ++i)
				lights[i].SetLightUniform(shader, i);

			shader.setVec3("viewPos", camera.position());

			//Clear
			glClearColor(33.f / 255.f, 146.f / 255.f, 248.f/255.f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//Draw
			shader.use();
			texture.Use();

			player.UpdateModels();
			player.Draw(shader);

			cube.UpdateModels();
			cube.Draw(shader);

			World::Draw(shader);

			glfwSwapBuffers(m_window);
		}
	}

	glfwTerminate();
}



Minecraft::~Minecraft()
{

}