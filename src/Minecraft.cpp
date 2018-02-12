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
		{ {-0.5f, -0.5f, -0.5f},	{ 0.f, 0.5f } },//bot
		{ { 0.5f,  0.5f, -0.5f },	{ 0.5f, 1.0f } },
		{ { 0.5f, -0.5f, -0.5f },	{ 0.5f, 0.5f } },
		{ { 0.5f,  0.5f, -0.5f },	{ 0.5f, 1.0f } },//Top
		{ { -0.5f, -0.5f, -0.5f },	{ 0.f, 0.5f } },
		{ { -0.5f,  0.5f, -0.5f },	{ 0.f, 1.0f } },

			//Front
		{ { -0.5f, -0.5f, 0.5f },	{ 0.f, 0.5f } },//bot
		{ { 0.5f, -0.5f, 0.5f },	{ 0.5f, 0.5f } },
		{ { 0.5f,  0.5f, 0.5f },	{ 0.5f, 1.0f } },
		{ { 0.5f,  0.5f, 0.5f },	{ 0.5f, 1.0f } },//Top
		{ { -0.5f,  0.5f, 0.5f },	{ 0.f, 1.0f } },
		{ { -0.5f, -0.5f, 0.5f },	{ 0.f, 0.5f } },

		//Left
		{ { -0.5f,  0.5f,  0.5f },	{ 0.f, 1.f } },//Top
		{ { -0.5f,  0.5f, -0.5f },	{ 0.5f, 1.f } },
		{ { -0.5f, -0.5f, -0.5f },	{ 0.5f, 0.5f } },
		{ { -0.5f, -0.5f, -0.5f },	{ 0.5f, 0.5f } },//Bot
		{ { -0.5f, -0.5f,  0.5f },	{ 0.f, 0.5f } },
		{ { -0.5f,  0.5f,  0.5f },	{ 0.f, 1.f } },

		//Right
		{ { 0.5f,  0.5f,  0.5f },	{ 0.f, 1.f } },//Top
		{ { 0.5f, -0.5f, -0.5f },	{ 0.5f, 0.5f } },
		{ { 0.5f,  0.5f, -0.5f },	{ 0.5f, 1.f } },
		{ { 0.5f, -0.5f, -0.5f },	{ 0.5f, 0.5f } },//Bot
		{ { 0.5f,  0.5f,  0.5f },	{ 0.f, 1.f } },
		{ { 0.5f, -0.5f,  0.5f },	{ 0.f, 0.5f } },

		//Bottom
		{ { -0.5f, -0.5f, -0.5f },	{ 0.5f, 1.0f } },
		{ { 0.5f, -0.5f, -0.5f },	{ 1.0f, 1.0f } },
		{ { 0.5f, -0.5f,  0.5f },	{ 1.0f, 0.5f } },
		{ { 0.5f, -0.5f,  0.5f },	{ 1.0f, 0.5f } },
		{ { -0.5f, -0.5f,  0.5f },	{ 0.5f, 0.5f } },
		{ { -0.5f, -0.5f, -0.5f },	{ 0.5f, 1.0f } },

		//Top
		{ { 0.5f,  0.5f, -0.5f },	{ 0.5f, 0.5f}},
		{ { -0.5f,  0.5f, -0.5f },	{ 0.0f, 0.5f } },
		{ { 0.5f,  0.5f,  0.5f },	{ 0.5f, 0.0f } },
		{ { -0.5f,  0.5f,  0.5f },	{ 0.0f, 0.0f } },
		{ { 0.5f,  0.5f,  0.5f },	{ 0.5f, 0.0f } },
		{ { -0.5f,  0.5f, -0.5f },	{ 0.0f, 0.5f } }
	};

	//Shader
	Shader shader("shaders/shader.vs", "shaders/shader.fs");
	Texture texture("textures/grass.bmp");
	Mesh mesh(vertices, texture);
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

		playerController.Update( 0.f );

		shader.setMat4("model", cube.ModelMatrix());
		shader.setMat4("view", camera.ViewMatrix());
		shader.setMat4("projection", camera.ProjectionMatrix());

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mesh.Draw( shader);

		glfwSwapBuffers(m_window);
		glfwPollEvents();
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
/*
vertices = verticesData;
indices = indicesData;

setupMesh();
}

void Mesh::Draw(Shader shader)
{
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	glActiveTexture(GL_TEXTURE0); // active proper texture unit before binding
	glUniform1i(glGetUniformLocation(shader.ID, "tex"), texture.textureId);
	glBindTexture(GL_TEXTURE_2D, texture.textureId);

	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// always good practice to set everything back to defaults once configured.
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set the vertex attribute pointers
	// vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);*/