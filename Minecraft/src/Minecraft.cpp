#include "Minecraft.h"

GLFWwindow* Minecraft::m_window = nullptr;

Minecraft::Minecraft(std::string name, int width, int height) :
	m_width(width),
	m_height(height),
	m_samples(8)
{
	// Setup window
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		return;

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
	#endif

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_width, m_height, name.c_str(), NULL, NULL);
	if (!m_window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return ;

	Input::Setup(m_window);
	ImGuiManager::Init(m_window);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSwapInterval(0);

	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	Time::SetDeltaTime( 1.f / (float)mode->refreshRate );
}

void Minecraft::Start()
{
	float t1 = Time::ElapsedSinceStartup();

	//Shaders 2D
	Shader shader_debug_ui("shaders/2D/debug_ui.vs", "shaders/2D/debug_ui.fs");
	Shader shader_Text("shaders/2D/text.vs", "shaders/2D/text.fs");
	Shader shader_draw_texture("shaders/2D/drawTexture.vs", "shaders/2D/drawTexture.fs");
	
	//Shaders deferred
	Shader shader_deferred_geometry("shaders/deferred/geometry.vs", "shaders/deferred/geometry.fs");
	Shader shader_deferred_light("shaders/deferred/light.vs", "shaders/deferred/light.fs");
	Shader shader_deferred_SSAO("shaders/deferred/ssao.vs", "shaders/deferred/ssao.fs");
	
	//Shaders forward
	Shader shader_debug("shaders/forward/debug.vs", "shaders/forward/debug.fs");
	Shader shader_skybox("shaders/forward/skybox.vs", "shaders/forward/skybox.fs");
	Shader shader_transparent("shaders/forward/transparent.vs", "shaders/forward/transparent.fs");
	
	//Shaders postprocess
	Shader shader_postprocess("shaders/postprocess/postprocess.vs", "shaders/postprocess/postprocess.fs");
	Shader shader_FXAA("shaders/postprocess/FXAA.vs", "shaders/postprocess/FXAA.fs");
	Shader shader_borders("shaders/postprocess/borders.vs", "shaders/postprocess/borders.fs");
	
	//Shaders test
	Shader shader_test("shaders/test.vs", "shaders/test.fs");

	GrayFBO fboSSAO(m_width/5, m_height/5);
	TextureDepthFBO fboBorders(m_width, m_height);
	TextureDepthFBO fboFXAA(m_width, m_height);
	DeferredFBO gBuffer(m_width, m_height);
	PostProcessingFBO fboPostProc(m_width, m_height);
	PostProcessingFBO fboPostProcTransparent(m_width, m_height);

	Tiles::Initialize(4, 4);
	TexturesBlocks::Initialize();
	Texture textureBlocks("textures/blocks.png");
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

	World::GenerateChunks();
	World::GeneratePhysics();

	
	glm::vec3 startPos(World::size * Chunck::size / 2, World::height * Chunck::size, World::size * Chunck::size / 2);
	PlayerAvatar player;
	player.rb().translate(btVector3(startPos.x, startPos.y, startPos.z));
	//player.rb().translate(btVector3(World::size * Chunck::size / 2, World::height * Chunck::size / 16, World::size * Chunck::size / 2));

	FreeCameraController freeCameraController( glm::vec2(m_width, m_height) );
	freeCameraController.SetEnabled(false);
	freeCameraController.GetCamera().SetPosition(startPos);
	PlayerController playerController(glm::vec2(m_width, m_height), player);
	playerController.SetEnabled(true);

	Camera* usedCamera = & freeCameraController.GetCamera();
	if( !freeCameraController.Enabled())
		usedCamera = & playerController.GetCamera();

	Cube cube;
	cube.rb().translate(btVector3(4.5, 20, 4.5));

	glm::vec3 sunDir = glm::normalize(glm::vec3(4, -4, 1));
	DirectionalLight sunLight(sunDir, 20.f);
	DirectionalLight sunLightLarge(sunDir, 100.f);

	float drawTimer = 0.f;
	float fixedUpdateTimer = 0.f;

	int frameCount = 0;
	float fpsDelta = 0.f;
	float fps = 0.f;
	float updateRate = 4.f;

	float time = Time::ElapsedSinceStartup();

	bool initShader = true;

	float t2 = Time::ElapsedSinceStartup();
	std::cout << 1000.f*(t2 - t1) << std::endl;

	bool multisample = true;
	bool viewFrustumCulling = true;
	bool vSync = true;

	//////////////////////////////////////////
	std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
	std::default_random_engine generator;
	std::vector<glm::vec3> ssaoKernel;
	ssaoKernel.reserve(64);
	for (unsigned int i = 0; i < 64; ++i)
	{
		glm::vec3 sample(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator)
		);
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = (float)i / 64.f;
		scale = glm::Lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}

	std::vector<glm::vec3> ssaoNoise;
	for (unsigned int i = 0; i < 16; i++)
	{
		glm::vec3 noise(
			randomFloats(generator) * 2.0 - 1.0,
			randomFloats(generator) * 2.0 - 1.0,
			0.0f);
		ssaoNoise.push_back(noise);
	}

	Texture ssaoNoiseTex(4, 4, ssaoNoise);

	//Imgui data
	std::stringstream ssItems;
	for (int i = 0; i < Block::count; ++i)
		ssItems << Block::typeName[i] << '\0';

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
				if (freeCameraController.Enabled())
				{
					freeCameraController.SetEnabled(false);
					playerController.SetEnabled(true);
					usedCamera = &playerController.GetCamera();
				}
				else
				{
					freeCameraController.SetEnabled(true);
					playerController.SetEnabled(false);
					usedCamera = &freeCameraController.GetCamera();
				}
				shader_debug.Use();
				shader_debug.setMat4("projection", usedCamera->projectionMatrix());
			}
			//Set Shaders
			if (Keyboard::KeyPressed(GLFW_KEY_F2) || initShader)
			{
				if (!initShader)
					Shader::ReloadAll();
				initShader = false;

				shader_Text.Use();
				shader_Text.setMat4("projection", textProjection);

				shader_debug.Use();
				shader_debug.setMat4("view", usedCamera->viewMatrix());
				shader_debug.setMat4("projection", usedCamera->projectionMatrix());
			}
			if (Keyboard::KeyPressed(GLFW_KEY_F3))
				Debug::SetActivated(!Debug::Activated());

			playerController.selectedBlock = ( playerController.selectedBlock + Mouse::DeltaScroll().y + Block::count - 1) % (Block::count - 1);


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
		if (drawTimer >= Time::DeltaTime() || !vSync)
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


			if(viewFrustumCulling)
				World::ClipChuncks(playerController.GetCamera());


			//////////////////////////////// BAKE SHADOWS ////////////////////////////////
			sunLight.BakeShadows(player.rb().Position());
			sunLightLarge.BakeShadows(player.rb().Position());

			//////////////////////////////// DEFERRED GEOMETRY ////////////////////////////////
			shader_deferred_geometry.Use();

			gBuffer.Use();
			gBuffer.Clear();

			textureBlocks.Use(TextureUnit::Unit0);
			shader_deferred_geometry.setInt("textureBlocks", 0);
			shader_deferred_geometry.setMat4("projview", usedCamera->projectionMatrix() * usedCamera->viewMatrix());

			player.UpdateModels();
			player.Draw(shader_deferred_geometry);
			cube.UpdateModels();
			cube.Draw(shader_deferred_geometry);

			World::DrawOpaque(shader_deferred_geometry);

			//////////////////////////////// SSAO ////////////////////////////////
			fboSSAO.Use();
			fboSSAO.Clear();
			shader_deferred_SSAO.Use();

			gBuffer.UseNormal(TextureUnit::Unit1);
			gBuffer.UsePosition(TextureUnit::Unit2);
			gBuffer.UseDepth(TextureUnit::Unit3);
			ssaoNoiseTex.Use(TextureUnit::Unit4);
			shader_deferred_SSAO.setInt("gNormal", 1);
			shader_deferred_SSAO.setInt("gPosition", 2);
			shader_deferred_SSAO.setInt("gDepth", 3);
			shader_deferred_SSAO.setInt("texNoise", 4);
			shader_deferred_SSAO.setMat4("projView", usedCamera->projectionMatrix() * usedCamera->viewMatrix());
			shader_deferred_SSAO.setVec3Array("samples[0]", ssaoKernel);
			shader_deferred_SSAO.setVec2("windowSize", glm::vec2(m_width, m_height));

			glDisable(GL_DEPTH_TEST);
			glBindVertexArray(postProcVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glEnable(GL_DEPTH_TEST);

			//////////////////////////////// DEFERRED LIGHT ////////////////////////////////
			fboPostProc.Use();
			fboPostProc.Clear();

			gBuffer.UseColor(TextureUnit::Unit0);
			gBuffer.UseNormal(TextureUnit::Unit1);
			gBuffer.UsePosition(TextureUnit::Unit2);
			sunLight.fbo.UseTexture(TextureUnit::Unit3);
			sunLightLarge.fbo.UseTexture(TextureUnit::Unit4);
			fboSSAO.UseTexture(TextureUnit::Unit5);

			shader_deferred_light.Use();
			shader_deferred_light.setInt("gColor", 0);
			shader_deferred_light.setInt("gNormal", 1);
			shader_deferred_light.setInt("gPosition", 2);
			shader_deferred_light.setInt("shadowMap", 3);
			shader_deferred_light.setInt("shadowMapLarge", 4);
			shader_deferred_light.setInt("ambientOcclusion", 5);
			shader_deferred_light.setVec3("lightDir", -sunDir);
			shader_deferred_light.setVec3("viewPos", usedCamera->position());
			//shader_deferred_light.setVec3("lightColor", glm::vec3(135.f / 255.f, 134.f / 255.f, 255.f / 255.f));
			shader_deferred_light.setVec3("lightColor", glm::vec3(190 / 255.f, 255 / 255.f, 255.f / 255.f));

			shader_deferred_light.setMat4("projectionViewLight", sunLight.ProjectionView());
			shader_deferred_light.setMat4("projectionViewLightLarge", sunLightLarge.ProjectionView());

			glDisable(GL_DEPTH_TEST);
			glBindVertexArray(postProcVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glEnable(GL_DEPTH_TEST);

			FBO::BlitDepth(gBuffer, fboPostProc);

			//////////////////////////////// FORWARD RENDERING ////////////////////////////////
			//Draw skybox
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			shader_skybox.Use();
			shader_skybox.setInt("skybox", 0);
			glm::vec3 oldPos = usedCamera->position();
			usedCamera->SetPosition({ 0,0,0 });
			shader_skybox.setMat4("projView", usedCamera->projectionMatrix()*usedCamera->viewMatrix());
			usedCamera->SetPosition(oldPos);
			cubeMap.UseTexture(TextureUnit::Unit0);
			glBindVertexArray(skyboxVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glDepthFunc(GL_LESS);

			fboPostProcTransparent.Use();
			fboPostProcTransparent.Clear();

			//Transparent objects
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glDepthFunc(GL_ALWAYS);
			shader_transparent.Use();
			textureBlocks.Use(TextureUnit::Unit0);
			fboPostProc.UseDepth(TextureUnit::Unit1);
			shader_transparent.setInt("textureBlocks", 0);
			shader_transparent.setInt("depthTexture", 1);
			shader_transparent.setVec3("lightDir", -sunDir);
			shader_transparent.setMat4("projView", usedCamera->projectionMatrix() * usedCamera->viewMatrix());
			World::DrawTransparent(shader_transparent);
			glDepthFunc(GL_LESS);


			//////////////////////////////// BLACK BORDERS ////////////////////////////////
			fboBorders.Use();
			fboBorders.Clear();

			shader_borders.Use();

			fboPostProc.UseDepth(TextureUnit::Unit0);
			fboPostProcTransparent.UseTexture(TextureUnit::Unit1);
			shader_borders.setInt("depthTexture", 0);
			shader_borders.setInt("transTexture", 1);

			glDisable(GL_DEPTH_TEST);
			glBindVertexArray(postProcVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glEnable(GL_DEPTH_TEST);

			//////////////////////////////// POSTPROCESSING ////////////////////////////////
			shader_postprocess.Use();

			fboFXAA.Use();
			fboFXAA.Clear();

			//effects
			fboPostProc.UseTexture(TextureUnit::Unit0);
			fboBorders.UseTexture(TextureUnit::Unit1);
			fboPostProcTransparent.UseTexture(TextureUnit::Unit2);
			shader_postprocess.setInt("screenTexture", 0);
			shader_postprocess.setInt("bordersTexture", 1);
			shader_postprocess.setInt("transtex", 2);
			glDisable(GL_DEPTH_TEST);
			glBindVertexArray(postProcVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glEnable(GL_DEPTH_TEST);


			//fxaa
			FBO::UseDefault();
			FBO::ClearDefault();
			shader_FXAA.Use();
			fboFXAA.UseTexture(TextureUnit::Unit0);

			shader_FXAA.setInt("colorTexture", 0);
			shader_FXAA.setFloat("FXAASpan", 8.0f);
			shader_FXAA.setFloat("FXAAReduceMul", 1.0f / 8.0f);
			shader_FXAA.setFloat("FXAAReduceMin", 1.0f / 128.0f);
			shader_FXAA.setBool("activated", multisample);

			glDisable(GL_DEPTH_TEST);
			glBindVertexArray(postProcVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glEnable(GL_DEPTH_TEST);

			//////////////////////////////// DEBUG ////////////////////////////////

			if (Debug::Activated())
			{
				glDisable(GL_DEPTH_TEST);
				shader_debug.Use();
				shader_debug.setMat4("view", usedCamera->viewMatrix());
				Debug::Draw(shader_debug, shader_debug_ui);
				glEnable(GL_DEPTH_TEST);
			}

			//////////////////////////////// IMGUI ////////////////////////////////
			ImGuiManager::NewFrame();

			//PERFORMANCE
			ImGui::Begin("Performance");
			ImGui::BulletText(" %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::BulletText(" %.1ik triangles", Statistics::GetTriangles() / 1000);
			ImGui::End();

			//BLOCKS
			ImGui::Begin("Blocks");
			ImGui::Combo("Block", &playerController.selectedBlock, ssItems.str().data(), Block::count-2);
			ImGui::End();

			//GRAPHICS
			ImGui::Begin("Graphics");
				if (ImGui::CollapsingHeader("Engine", ImGuiTreeNodeFlags_DefaultOpen))
				{
					//Multisampling
					bool oldValueMultisample = multisample;
					ImGui::Checkbox("FXAA", &multisample);
					if (oldValueMultisample != multisample)
					{
						if (multisample)
							glEnable(GL_MULTISAMPLE);
						else
							glDisable(GL_MULTISAMPLE);
					}

					//View frustum culling
					bool oldValueviewFrustumCulling = viewFrustumCulling;
					ImGui::Checkbox("View frustum culling", &viewFrustumCulling);
					if (oldValueviewFrustumCulling != viewFrustumCulling && !viewFrustumCulling)
						World::EnableAllChuncks();
				}

				if (ImGui::CollapsingHeader("OpenGl", ImGuiTreeNodeFlags_DefaultOpen))
				{
					//vSync
					ImGui::Checkbox("vSync", &vSync);

				}
			ImGui::End();

			ImGui::Render();

			/////////////////////////////// DRAW ////////////////////////////////
			glfwSwapBuffers(m_window);
			Debug::Clear();
		}
	}

	ImGuiManager::Shutdown();
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
	const float size = 1.f;
	float skyboxVertices[] = {
		// positions          
		-size,  size, -size,
		-size, -size, -size,
		size, -size, -size,
		size, -size, -size,
		size,  size, -size,
		-size,  size, -size,

		-size, -size,  size,
		-size, -size, -size,
		-size,  size, -size,
		-size,  size, -size,
		-size,  size,  size,
		-size, -size,  size,

		size, -size, -size,
		size, -size,  size,
		size,  size,  size,
		size,  size,  size,
		size,  size, -size,
		size, -size, -size,

		-size, -size,  size,
		-size,  size,  size,
		size,  size,  size,
		size,  size,  size,
		size, -size,  size,
		-size, -size,  size,

		-size,  size, -size,
		size,  size, -size,
		size,  size,  size,
		size,  size,  size,
		-size,  size,  size,
		-size,  size, -size,

		-size, -size, -size,
		-size, -size,  size,
		size, -size, -size,
		size, -size, -size,
		-size, -size,  size,
		size, -size,  size
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