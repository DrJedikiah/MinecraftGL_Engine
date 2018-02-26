#include "Minecraft.h"

GLFWwindow* Minecraft::m_window = nullptr;

Minecraft::Minecraft(std::string name, int width, int height):
	m_width(width),
	m_height(height)
{

	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
	#endif

	// glfw window creation
	m_window = glfwCreateWindow(m_width, m_height, name.c_str(), NULL, NULL);
	if (m_window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_window);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	Input::Setup(m_window);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		return;
	}

	//Setup openGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSwapInterval(0);
}

void Minecraft::Start()
{
	float t1 = Time::ElapsedSinceStartup();
	//Shader
	Shader shader_debug("shaders/shader_debug.vs", "shaders/shader_debug.fs");
	Shader shader_debug_ui("shaders/shader_debug_ui.vs", "shaders/shader_debug_ui.fs");
	
	Shader shader("shaders/shader.vs", "shaders/shader.fs");

	Shader shaderText("shaders/shader_text.vs", "shaders/shader_text.fs");
	glm::mat4 orthoProj = glm::ortho(0.0f, static_cast<GLfloat>(m_width), 0.0f, static_cast<GLfloat>(m_height));
	shaderText.setMat4("projection", orthoProj);

	Texture texture("textures/grass.bmp");

	Font font("fonts/arial.ttf", 48);

	//Block Tiles and textures
	Tiles::Initialize(4,4);
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
	Camera camera(m_width, m_height, 1000);
	camera.Translate(glm::vec3(14, 16, 14 + 8) - camera.position());
	camera.RotateUp(glm::radians(-15.f));

	PlayerAvatar player;
	//player.rb().translate(btVector3(8, 4*16+2, 8));
	
	player.rb().translate(btVector3(World::size * Chunck::size/2, World::height * Chunck::size, World::size * Chunck::size/2));

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

	shader_debug.use();
	shader_debug.setMat4("view", camera.viewMatrix());
	shader_debug.setMat4("projection", camera.projectionMatrix());

	float drawTimer= 0.f;
	float fixedUpdateTimer = 0.f;

	
	int frameCount = 0;
	float fpsDelta = 0.f;
	float fps= 0.f;
	float updateRate = 4.f;
	
	float time = Time::ElapsedSinceStartup();

	float t2 = Time::ElapsedSinceStartup();
	std::cout << 1000.f*(t2 - t1) << std::endl;

	// render loop
	while (!glfwWindowShouldClose(m_window))
	{
		float delta = Time::ElapsedSinceStartup() - time;
		time += delta;

		fpsDelta += delta;

		//Fixed update
		fixedUpdateTimer += delta;
		if (fixedUpdateTimer >= Time::FixedDeltaTime())
		{
			Input::Update();

			//Toogle controllers
			if (Keyboard::KeyPressed(GLFW_KEY_F1))
			{
				freeCameraController.SetEnabled( ! freeCameraController.Enabled() );
				playerController.SetEnabled( ! playerController.Enabled() );
			}
			if (Keyboard::KeyPressed(GLFW_KEY_ESCAPE))
				glfwSetWindowShouldClose(m_window, true);
				

			PhysicsEngine::StepSimulation(fixedUpdateTimer);

			freeCameraController.Update(fixedUpdateTimer);
			playerController.Update(fixedUpdateTimer);

			player.Update(fixedUpdateTimer);
			cube.Update(fixedUpdateTimer);
			World::Update(fixedUpdateTimer);

			fixedUpdateTimer = 0.f;
		}

		//Draws
		
		drawTimer += delta;
		//if (drawTimer >= Time::DeltaTime())
		{
			//Fps count
			++frameCount;
			if (fpsDelta > 1.0 / updateRate)
			{
				fps = frameCount / fpsDelta;
				frameCount = 0;
				fpsDelta -= 1.f / updateRate;
			}

			drawTimer = 0.f;
			//Clear
			glClearColor(33.f / 255.f, 146.f / 255.f, 248.f/255.f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			//Draw world
			shader.use();
			texture.Use();
			shader.setMat4("view", camera.viewMatrix());
			for (int i = 0; i < (int)lights.size(); ++i)
				lights[i].SetLightUniform(shader, i);
			shader.setVec3("viewPos", camera.position());
			player.UpdateModels();
			player.Draw(shader);
			cube.UpdateModels();
			cube.Draw(shader);
			World::Draw(shader);

			//Debug
			shader_debug.use();
			shader_debug.setMat4("view", camera.viewMatrix());
			Debug::Draw(shader_debug, shader_debug_ui);

			std::stringstream ss;
			ss << (int)fps;
			font.RenderText(shaderText, ss.str() , 0, (GLfloat) m_height - 40);

			glfwSwapBuffers(m_window);

			Debug::Clear();
		}
	}

	glfwTerminate();
}



Minecraft::~Minecraft()
{

}