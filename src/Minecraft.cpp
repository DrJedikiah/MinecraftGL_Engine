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
	//Shader
	Shader shader("shaders/shader.vs", "shaders/shader.fs");
	Texture texture("textures/grass.bmp");

	//Block Tiles
	BlockTiles::Initialize(2,2);
	BlockTiles::SetBlockTile(BlockTiles::dirt, 1, 1);
	BlockTiles::SetBlockTile(BlockTiles::grassTop, 0, 0);
	BlockTiles::SetBlockTile(BlockTiles::grassSide, 0, 1);

	std::vector<Light> lights =
	{
		Light({ 30,30,14.5 }),
	};

	shader.use();
	texture.Use();

	Physics physicsEngine;
	World world;
	world.GeneratePhysics(physicsEngine);
	Camera camera(m_scr_width, m_scr_height, 100);
	PlayerController playerController(camera);
	camera.translate(glm::vec3(14, 16, 14 + 8) - camera.Position());
	camera.rotateUp(glm::radians(-15.f));
	btTransform transform = btTransform::getIdentity();
	transform.setOrigin(btVector3(14,16,14.5));
	btRigidBody * rb = physicsEngine.CreateRigidBody(1, transform, new btBoxShape(btVector3(0.5f, 0.5f, 0.5f)));
	transform.setOrigin(btVector3(13.6, 15.5, 14));
	btRigidBody * rb2 = physicsEngine.CreateRigidBody(1, transform, new btBoxShape(btVector3(0.5f, 0.5f, 0.5f)));
	transform.setOrigin(btVector3(14, 13.5, 14.5));
	btRigidBody * rb3 = physicsEngine.CreateRigidBody(1, transform, new btBoxShape(btVector3(0.5f, 0.5f, 0.5f)));
	
	Model  cube({ Mesh(Util::DirtCubeMesh(1.f, 0.f, 0.f, 0.f)) });
	Model  cube2({ Mesh(Util::DirtCubeMesh(1.f, 0.f, 0.f, 0.f)) });
	Model  cube3({ Mesh(Util::DirtCubeMesh(1.f, 0.f, 0.f, 0.f)) });

	shader.setFloat("ambient", 0.2f);
	shader.setMat4("view", camera.ViewMatrix());
	shader.setMat4("projection", camera.ProjectionMatrix());
	shader.setInt("numLights", lights.size());

	float drawTimer = 0.f;
	float fixedUpdateTimer = 0.f;
	float time = Time::ElapsedSinceStartup();

	// render loop
	while (!glfwWindowShouldClose(m_window))
	{
		float delta = Time::ElapsedSinceStartup() - time;
		time += delta;

		//Update
		glfwPollEvents();
		if( Keyboard::IsKeyDown(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(m_window, true);
		if (Keyboard::IsKeyDown(GLFW_KEY_C))
		{
			transform.setOrigin(btVector3(16.6f, 30, 16.6f));
			rb->setWorldTransform(transform);
		}
		
		//Fixed update
		fixedUpdateTimer += delta;
		if (fixedUpdateTimer >= Time::FixedDeltaTime())
		{
			physicsEngine.StepSimulation(0.01f);
			Mouse::Update();
			fixedUpdateTimer = 0.f;
			playerController.Update(Time::FixedDeltaTime());
			
			//Cube
			btTransform trans;
			rb->getMotionState()->getWorldTransform(trans);
			glm::vec3 euler;
			trans.getRotation().getEulerZYX(euler.z, euler.y, euler.x);
			cube.SetPosition({ 
				trans.getOrigin().getX(),
				trans.getOrigin().getY(),
				trans.getOrigin().getZ() 
			});
			cube.SetRotation(euler);

			//Cube
			rb2->getMotionState()->getWorldTransform(trans);
			trans.getRotation().getEulerZYX(euler.z, euler.y, euler.x);
			cube2.SetPosition({
				trans.getOrigin().getX(),
				trans.getOrigin().getY(),
				trans.getOrigin().getZ()
				});
			cube2.SetRotation(euler);

			//Cube
			rb3->getMotionState()->getWorldTransform(trans);
			trans.getRotation().getEulerZYX(euler.z, euler.y, euler.x);
			cube3.SetPosition({
				trans.getOrigin().getX(),
				trans.getOrigin().getY(),
				trans.getOrigin().getZ()
				});
			cube3.SetRotation(euler);
		}

		//Draws
		drawTimer += delta;
		if (drawTimer >= Time::DeltaTime())
		{
			drawTimer = 0.f;

			//Uniforms
			shader.setMat4("view", camera.ViewMatrix());
			for (int i = 0; i < (int)lights.size(); ++i)
				lights[i].SetLightUniform(shader, i);

			shader.setVec3("viewPos", camera.Position());

			//Clear
			glClearColor(33.f / 255.f, 146.f / 255.f, 248.f/255.f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			

			//Draw
			shader.use();
			texture.Use();
			cube.Draw(shader);
			cube2.Draw(shader);
			cube3.Draw(shader);
			world.Draw(shader);

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