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
	glLineWidth(2);

	glfwSwapInterval(0);
}

void Minecraft::Start()
{
	float t1 = Time::ElapsedSinceStartup();

	//Shader
	Shader shader_postprocess("shaders/postprocess.vs", "shaders/postprocess.fs");
	Shader shader_debug("shaders/debug.vs", "shaders/debug.fs");
	Shader shader_debug_ui("shaders/debug_ui.vs", "shaders/debug_ui.fs");
	Shader shader_skybox("shaders/skybox.vs", "shaders/skybox.fs");
	Shader shader_Text("shaders/text.vs", "shaders/text.fs");
	Shader shader_shadows("shaders/shadows.vs", "shaders/shadows.fs");
	Shader shader_deferred_geometry("shaders/deferred_geometry.vs", "shaders/deferred_geometry.fs");
	Shader shader_deferred_light("shaders/deferred_light.vs", "shaders/deferred_light.fs");
	Shader shader_deferred_SSAO("shaders/deferred_ssao.vs", "shaders/deferred_ssao.fs");
	Shader shader_deferred_borders("shaders/deferred_borders.vs", "shaders/deferred_borders.fs");
	Shader shader_draw_texture("shaders/drawTexture.vs", "shaders/drawTexture.fs");
	Shader shader_test("shaders/test.vs", "shaders/test.fs");
	Shader shader_FXAA("shaders/FXAA.vs", "shaders/FXAA.fs");


	GrayFBO fboSSAO(m_width/5, m_height/5);
	GrayFBO fboBorders(m_width, m_height);
	TextureDepthFBO fboFXAA(m_width, m_height);
	ShadowMapFBO fboShadow(2 * 2048, 2 * 2048);
	ShadowMapFBO fboShadowLarge(2 * 2048, 2 * 2048);
	DeferredFBO gBuffer(m_width, m_height);
	PostProcessingFBO fboPostProc(m_width, m_height);

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
	glm::mat4 lightProjection = glm::ortho(-20.f, 20.f, -15.f, 15.f, 0.1f, 1000.f);
	glm::mat4 lightProjectionLarge = glm::ortho(-100.f, 100.f, -100.f, 100.f, 0.1f, 1000.f);


	World::GenerateChunks();
	World::GeneratePhysics();

	Camera camera(m_width, m_height, 1000.f, 0.2f);

	PlayerAvatar player;
	player.rb().translate(btVector3(World::size * Chunck::size / 2, World::height * Chunck::size, World::size * Chunck::size / 2));
	//player.rb().translate(btVector3(World::size * Chunck::size / 2, World::height * Chunck::size / 16, World::size * Chunck::size / 2));

	FreeCameraController freeCameraController(camera);
	freeCameraController.SetEnabled(true);
	PlayerController playerController(camera, player);
	playerController.SetEnabled(false);

	Cube cube;
	cube.rb().translate(btVector3(4.5, 20, 4.5));

	Light sunLight
	{
		(float)Chunck::size * glm::vec3((float)World::size / 4.F, 2 * World::height ,World::size)
	};

	Model sunModel(Cube::CreateCubeMesh(10));
	sunModel.Translate(sunLight.position);

	float drawTimer = 0.f;
	float fixedUpdateTimer = 0.f;

	int frameCount = 0;
	float fpsDelta = 0.f;
	float fps = 0.f;
	float updateRate = 4.f;

	float time = Time::ElapsedSinceStartup();

	bool initShader = true;
	bool debugActivated = true;

	float t2 = Time::ElapsedSinceStartup();
	std::cout << 1000.f*(t2 - t1) << std::endl;

	bool multisample = true;

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

	bool show_another_window = false;
	ImVec4 clear_color = ImColor(114, 144, 154);
	char text[64] = "";

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
				freeCameraController.SetEnabled(!freeCameraController.Enabled());
				playerController.SetEnabled(!playerController.Enabled());
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
				shader_debug.setMat4("view", camera.viewMatrix());
				shader_debug.setMat4("projection", camera.projectionMatrix());
			}
			if (Keyboard::KeyPressed(GLFW_KEY_F3))
				debugActivated = !debugActivated;
			if (Keyboard::KeyPressed(GLFW_KEY_F4))
			{
				multisample = !multisample;
				if (multisample)
					glEnable(GL_MULTISAMPLE);
				else
					glDisable(GL_MULTISAMPLE);
			}

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
					glm::vec3(posPlayer.getX(), posPlayer.getY(), posPlayer.getZ()),
					glm::vec3(0.0f, 1.0f, 0.0f));

			shader_shadows.Use();
			//////
			fboShadow.Use();
			fboShadow.Clear();
			shader_shadows.setMat4("projview", lightProjection*lightView);
			World::Draw(shader_shadows);
			//////
			fboShadowLarge.Use();
			fboShadowLarge.Clear();
			shader_shadows.setMat4("projview", lightProjectionLarge*lightView);
			World::Draw(shader_shadows);

			//////////////////////////////// DEFERRED GEOMETRY ////////////////////////////////
			shader_deferred_geometry.Use();

			gBuffer.Use();
			gBuffer.Clear();

			texture.Use(TextureUnit::Unit0);
			shader_deferred_geometry.setInt("textureBlocks", 0);
			shader_deferred_geometry.setMat4("projview", camera.projectionMatrix() * camera.viewMatrix());

			player.UpdateModels();
			player.Draw(shader_deferred_geometry);
			cube.UpdateModels();
			cube.Draw(shader_deferred_geometry);

			World::Draw(shader_deferred_geometry);

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
			shader_deferred_SSAO.setMat4("projView", camera.projectionMatrix() * camera.viewMatrix());
			shader_deferred_SSAO.setVec3Array("samples[0]", ssaoKernel);
			shader_deferred_SSAO.setVec2("windowSize", glm::vec2(m_width, m_height));

			glDisable(GL_DEPTH_TEST);
			glBindVertexArray(postProcVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glEnable(GL_DEPTH_TEST);

			//////////////////////////////// BLACK BORDERS ////////////////////////////////
			fboBorders.Use();
			fboBorders.Clear();
			shader_deferred_borders.Use();

			gBuffer.UseDepth(TextureUnit::Unit0);
			shader_deferred_borders.setInt("gDepth", 0);

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
			shader_deferred_light.setVec3("lightColor", glm::vec3(135.f / 255.f, 134.f / 255.f, 255.f / 255.f));
			shader_deferred_light.setMat4("projectionViewLight", lightProjection*lightView);
			shader_deferred_light.setMat4("projectionViewLightLarge", lightProjectionLarge*lightView);

			glDisable(GL_DEPTH_TEST);
			glBindVertexArray(postProcVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glEnable(GL_DEPTH_TEST);

			FBO::BlitDepth(gBuffer, fboPostProc);

			//////////////////////////////// FORWARD RENDERING ////////////////////////////////
			if (debugActivated)
			{
				shader_test.Use();
				shader_test.setMat4("view", camera.viewMatrix());
				shader_test.setMat4("projection", camera.projectionMatrix());
				sunModel.Draw(shader_test);
			}

			//Draw skybox
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			shader_skybox.Use();
			shader_skybox.setInt("skybox", 0);
			glm::vec3 oldPos = camera.position();
			camera.SetPosition({ 0,0,0 });
			shader_skybox.setMat4("projView", camera.projectionMatrix()*camera.viewMatrix());
			camera.SetPosition(oldPos);
			cubeMap.UseTexture(TextureUnit::Unit0);
			glBindVertexArray(skyboxVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glDepthFunc(GL_LESS);

			//////////////////////////////// POSTPROCESSING ////////////////////////////////
			shader_postprocess.Use();

			fboFXAA.Use();
			fboFXAA.Clear();

			fboPostProc.UseTexture(TextureUnit::Unit0);
			fboBorders.UseTexture(TextureUnit::Unit1);
			fboSSAO.UseTexture(TextureUnit::Unit2);
			gBuffer.UseDepth(TextureUnit::Unit3);
			shader_postprocess.setInt("screenTexture", 0);
			shader_postprocess.setInt("bordersTexture", 1);
			shader_postprocess.setInt("ambientOcclusion", 2);
			shader_postprocess.setInt("depth", 3);
			glDisable(GL_DEPTH_TEST);
			glBindVertexArray(postProcVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glEnable(GL_DEPTH_TEST);


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

			//////////////////////////////// UI ////////////////////////////////

			if (debugActivated)
			{
				glDisable(GL_DEPTH_TEST);
				//FPS counter
				std::stringstream ss;
				ss << "fps: " << (int)fps;
				font.RenderText(shader_Text, ss.str(), 0, (GLfloat)m_height - 20);

				//Triangles counter
				std::stringstream ss2;
				ss2 << "triangles : " << Statistics::GetTriangles() / 1000 << "k";
				font.RenderText(shader_Text, ss2.str(), 0, (GLfloat)(m_height - 40));

				//ui
				shader_debug.Use();
				shader_debug.setMat4("view", camera.viewMatrix());
				Debug::Draw(shader_debug, shader_debug_ui);
				glEnable(GL_DEPTH_TEST);
			}

			//////////////////////////////// DRAW ////////////////////////////////
			ImGuiManager::NewFrame();

			// 1. Show a simple window
			// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
			{
				static float f = 0.0f;
				ImGui::Text("Hello, world!");
				ImGui::InputText("input text", text, 64, ImGuiInputTextFlags_AutoSelectAll);
				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
				ImGui::ColorEdit3("clear color", (float*)&clear_color);
				if (ImGui::Button("Another Window")) show_another_window ^= 1;
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			// 2. Show another simple window, this time using an explicit Begin/End pair
			if (show_another_window)
			{
				ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
				ImGui::Begin("Another Window", &show_another_window);
				ImGui::Text("Hello");
				ImGui::End();
			}

			// Rendering

			ImGui::Render();

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