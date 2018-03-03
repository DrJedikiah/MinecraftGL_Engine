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
	glfwWindowHint(GLFW_SAMPLES, 4);

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
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);

	glfwSwapInterval(0);
}

void Minecraft::Start()
{
	float t1 = Time::ElapsedSinceStartup();

	//Shader
	Shader shader_postprocess("shaders/postprocess.vs", "shaders/postprocess.fs");
	Shader shader_debug("shaders/debug.vs", "shaders/debug.fs");
	Shader shader_debug_ui("shaders/debug_ui.vs", "shaders/debug_ui.fs");
	Shader shader("shaders/forward_map.vs", "shaders/forward_map.fs");
	Shader shader_skybox("shaders/skybox.vs", "shaders/skybox.fs");
	Shader shaderText("shaders/text.vs", "shaders/text.fs");
	Shader shader_shadows("shaders/shadows.vs", "shaders/shadows.fs");
	Shader shader_deferred("shaders/deferred_map.vs", "shaders/deferred_map.fs");
	Shader shader_deferred_light("shaders/deferred_light.vs", "shaders/deferred_light.fs");
	Shader shader_test("shaders/test.vs", "shaders/test.fs");

	FrameBuffer fboPostProc(m_width, m_height);
	FrameBufferShadowMap fboShadow(4*2048, 4*2048);
	FrameBufferShadowMap fboShadowLarge(4 * 2048, 4 * 2048);
	GBuffer gBuffer(m_width, m_height);


	Tiles::Initialize(4, 4);
	TexturesBlocks::Initialize();
	Texture texture("textures/grass.png");
	Font font("fonts/arial.ttf", 20);
	CubeMap cubeMap({
		"textures/skybox/right.png",
		"textures/skybox/left.png",
		"textures/skybox/up.png",
		"textures/skybox/down.png",
		"textures/skybox/front.png",
		"textures/skybox/back.png"
		});
	
	SetupPostProcess();
	SetupSkyBox();
	
	glm::mat4 textProjection = glm::ortho(0.0f, static_cast<GLfloat>(m_width), 0.0f, static_cast<GLfloat>(m_height));
	glm::mat4 lightProjection = glm::ortho(-20.f, 20.f, -20.f, 20.f, 0.1f, 1000.f);
	glm::mat4 lightProjectionLarge = glm::ortho(-100.f, 100.f, -100.f, 100.f, 0.1f, 1000.f);

	World::GenerateChunks();
	World::GeneratePhysics();

	Camera camera(m_width, m_height, 1000.f, 0.2f);

	PlayerAvatar player;
	player.rb().translate(btVector3(World::size * Chunck::size/2, World::height * Chunck::size, World::size * Chunck::size/2));

	FreeCameraController freeCameraController( camera);
	freeCameraController.SetEnabled(false);
	PlayerController playerController(camera, player);
	playerController.SetEnabled(true);
	
	Cube cube;
	cube.rb().translate(btVector3(4.5, 20, 4.5));
	
	Light sunLight{ glm::vec3(500,300,500) };
	Model sunModel(Cube::CreateCubeMesh(10));
	sunModel.Translate(sunLight.position);

	float drawTimer= 0.f;
	float fixedUpdateTimer = 0.f;
	
	int frameCount = 0;
	float fpsDelta = 0.f;
	float fps= 0.f;
	float updateRate = 4.f;
	
	float time = Time::ElapsedSinceStartup();

	bool initShader = true;
	bool debugActivated = true;

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


			//Exit app
			if (Keyboard::KeyPressed(GLFW_KEY_ESCAPE))
				glfwSetWindowShouldClose(m_window, true);

			//Toogle controllers
			if (Keyboard::KeyPressed(GLFW_KEY_F1))
			{
				freeCameraController.SetEnabled( ! freeCameraController.Enabled() );
				playerController.SetEnabled( ! playerController.Enabled() );
			}
			//Set Shaders
			if (Keyboard::KeyPressed(GLFW_KEY_F2) || initShader)
			{
				if (!initShader)
					Shader::ReloadAll();
				initShader = false;

				shaderText.Use();
				shaderText.setMat4("projection", textProjection);

				shader.Use();
				shader.setFloat("ambient", 0.2f);
				shader.setMat4("view", camera.viewMatrix());
				shader.setMat4("projection", camera.projectionMatrix());
				shader.setVec3("lightColor", glm::vec3(228.f / 256.f, 134 / 256.f, 0));

				shader_debug.Use();
				shader_debug.setMat4("view", camera.viewMatrix());
				shader_debug.setMat4("projection", camera.projectionMatrix());
			}
			if (Keyboard::KeyPressed(GLFW_KEY_F3))
				debugActivated = !debugActivated;
				
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
		if (drawTimer >= Time::DeltaTime())
		{
			drawTimer = 0.f;

			//Fps count
			++frameCount;
			if (fpsDelta > 1.0 / updateRate)
			{
				fps = frameCount / fpsDelta;
				frameCount = 0;
				fpsDelta -= 1.f / updateRate;
			}

			//////////////////////////////// BAKE SHADOWS ////////////////////////////////
			btVector3 posPlayer = player.rb().transform().getOrigin();
			glm::mat4 lightView =
			glm::lookAt(
			sunLight.position,
			//camera.position(),
			glm::vec3(posPlayer.getX(), posPlayer.getY(), posPlayer.getZ()),
			glm::vec3(0.0f, 1.0f, 0.0f));

			shader_shadows.Use();
			shader_shadows.setMat4("view", lightView);
			//////
			fboShadow.Use();
			shader_shadows.setMat4("projection", lightProjection);
			World::Draw(shader_shadows);

			//////
			fboShadowLarge.Use();
			shader_shadows.setMat4("projection", lightProjectionLarge);
			World::Draw(shader_shadows);

			//////////////////////////////// DEFERRED GEOMETRY ////////////////////////////////
			shader_deferred.Use();

			glBindFramebuffer(GL_FRAMEBUFFER, gBuffer.buffer);
			glViewport(0, 0, m_width, m_height);
			glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			texture.Use(TextureUnit::Unit0);
			shader_deferred.setInt("textureBlocks", 0);
			shader_deferred.setMat4("view", camera.viewMatrix());
			shader_deferred.setMat4("projection", camera.projectionMatrix());
			
			player.UpdateModels();
			player.Draw(shader_deferred);
			cube.UpdateModels();
			cube.Draw(shader_deferred);
			if (debugActivated)
				sunModel.Draw(shader_deferred);
			World::Draw(shader_deferred);

			//////////////////////////////// DEFERRED LIGHT ////////////////////////////////
			fboPostProc.Use();
			//FrameBuffer::UseDefault();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gBuffer.gColor);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, gBuffer.gNormal);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, gBuffer.gPosition);
			fboShadow.UseTexture(TextureUnit::Unit3);
			fboShadowLarge.UseTexture(TextureUnit::Unit4);

			shader_deferred_light.Use();
			shader_deferred_light.setInt("gColor", 0);
			shader_deferred_light.setInt("gNormal", 1);
			shader_deferred_light.setInt("gPosition", 2);
			shader_deferred_light.setInt("shadowMap", 3);
			shader_deferred_light.setInt("shadowMapLarge", 4);
			shader_deferred_light.setVec3("lightPos", sunLight.position);
			shader_deferred_light.setVec3("viewPos", camera.position());
			shader_deferred_light.setVec3("lightColor", glm::vec3(228.f / 256.f, 134 / 256.f, 0));
			shader_deferred_light.setMat4("viewLight", lightView);
			shader_deferred_light.setMat4("projectionLight", lightProjection);
			shader_deferred_light.setMat4("projectionLightLarge", lightProjectionLarge);

			glDisable(GL_DEPTH_TEST);
			glClearColor(1,0,0, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBindVertexArray(postProcVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			//Copy depth data
			glEnable(GL_DEPTH_TEST);
			glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer.buffer);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboPostProc.fbo);
			glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

			//////////////////////////////// FORWARD RENDERING ////////////////////////////////
			shader_test.Use();
			shader_test.setMat4("view", camera.viewMatrix());
			shader_test.setMat4("projection", camera.projectionMatrix());

			//Draw skybox
			glDepthFunc(GL_LEQUAL);
			shader_skybox.Use();
			shader_skybox.setInt("skybox", 0);
			glm::vec3 oldPos = camera.position();
			camera.SetPosition({ 0,0,0 });
			shader_skybox.setMat4("view", camera.viewMatrix());
			shader_skybox.setMat4("projection", camera.projectionMatrix());
			camera.SetPosition(oldPos);
			glBindVertexArray(skyboxVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap.textureID);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glDepthFunc(GL_LESS);
			
			//////////////////////////////// POSTPROCESSING ////////////////////////////////
			shader_postprocess.Use();
			FrameBuffer::UseDefault();

			glDisable(GL_DEPTH_TEST);
			//glClearColor(33.f / 255.f, 146.f / 255.f, 248.f / 255.f, 1.0f);
			glClearColor(0, 1.0f, 1, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			fboPostProc.UseTexture(TextureUnit::Unit0);
			shader_postprocess.setInt("screenTexture", 0);

			glBindVertexArray(postProcVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glEnable(GL_DEPTH_TEST);

			//////////////////////////////// UI ////////////////////////////////

			if (debugActivated)
			{
				//FPS counter
				std::stringstream ss;
				ss << "fps: " << (int)fps;
				font.RenderText(shaderText, ss.str(), 0, (GLfloat)m_height - 20);

				//Triangles counter
				std::stringstream ss2;
				ss2 << "triangles : " << Statistics::GetTriangles() / 1000 << "k";
				font.RenderText(shaderText, ss2.str(), 0, (GLfloat)(m_height - 40));

				//ui
				shader_debug.Use();
				shader_debug.setMat4("view", camera.viewMatrix());
				Debug::Draw(shader_debug, shader_debug_ui);
			}

			//////////////////////////////// DRAW ////////////////////////////////

			glfwSwapBuffers(m_window);
			Debug::Clear();
		}
	}
	glfwTerminate();
}

void Minecraft::SetupPostProcess()
{
	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};
	glGenVertexArrays(1, &postProcVAO);
	glGenBuffers(1, &postProcVBO);
	glBindVertexArray(postProcVAO);
	glBindBuffer(GL_ARRAY_BUFFER, postProcVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

}

void Minecraft::SetupSkyBox()
{
	float skyboxVertices[] = {
		// positions          
		-1.f,  1.f, -1.f,
		-1.f, -1.f, -1.f,
		1.f, -1.f, -1.f,
		1.f, -1.f, -1.f,
		1.f,  1.f, -1.f,
		-1.f,  1.f, -1.f,

		-1.f, -1.f,  1.f,
		-1.f, -1.f, -1.f,
		-1.f,  1.f, -1.f,
		-1.f,  1.f, -1.f,
		-1.f,  1.f,  1.f,
		-1.f, -1.f,  1.f,

		1.f, -1.f, -1.f,
		1.f, -1.f,  1.f,
		1.f,  1.f,  1.f,
		1.f,  1.f,  1.f,
		1.f,  1.f, -1.f,
		1.f, -1.f, -1.f,

		-1.f, -1.f,  1.f,
		-1.f,  1.f,  1.f,
		1.f,  1.f,  1.f,
		1.f,  1.f,  1.f,
		1.f, -1.f,  1.f,
		-1.f, -1.f,  1.f,

		-1.f,  1.f, -1.f,
		1.f,  1.f, -1.f,
		1.f,  1.f,  1.f,
		1.f,  1.f,  1.f,
		-1.f,  1.f,  1.f,
		-1.f,  1.f, -1.f,

		-1.f, -1.f, -1.f,
		-1.f, -1.f,  1.f,
		1.f, -1.f, -1.f,
		1.f, -1.f, -1.f,
		-1.f, -1.f,  1.f,
		1.f, -1.f,  1.f
	};

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}

Minecraft::~Minecraft()
{

}