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
}

void Minecraft::Start()
{
	//Shader
	Shader shader("shaders/shader.vs", "shaders/shader.fs");

	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.f,1.f, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.f, 0.5f, // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f, 0.5f, // bottom left
		
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.f, 1.f, // top right
		-0.5f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  0.5f, 1.f, // top left
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.5f ,0.5f, // bottom left
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	Texture texture("textures/grass.bmp");
	Camera camera(m_scr_width, m_scr_height);
	PlayerController playerController(camera);
	GameObject cube;
	cube.translate(glm::vec3(0, 0, -10));

	shader.setInt("ourTexture", 0);
	shader.setMat4("model", cube.ModelMatrix());
	shader.setMat4("view", camera.ViewMatrix());
	shader.setMat4("projection", camera.ProjectionMatrix());

	// render loop
	while (!glfwWindowShouldClose(m_window))
	{

		Mouse::Update();
		if( Keyboard::IsKeyDown(GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(m_window, true);

		playerController.Update( 0. );

		shader.setMat4("model", cube.ModelMatrix());
		shader.setMat4("view", camera.ViewMatrix());
		shader.setMat4("projection", camera.ProjectionMatrix());

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture.textureId);
		shader.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.textureId);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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