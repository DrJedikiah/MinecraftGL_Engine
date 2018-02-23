#include "util/Debug.h"

Debug Debug::m_instance = Debug();
std::vector<Debug::Vertex> Debug::m_lines;

Debug::Debug()
{
	
}

void Debug::Clear()
{
	m_lines.clear();
}

void Debug::DrawLine(btVector3 start, btVector3 end, btVector3 color)
{
	m_lines.push_back({ start, color });
	m_lines.push_back({ end,   color });
}

void Debug::DrawCross(btVector3 position, float size, btVector3 color)
{
	Debug::DrawLine(position + btVector3(0, size, 0), position + btVector3(0, -size, 0), color);
	Debug::DrawLine(position + btVector3(size, 0, 0), position + btVector3(-size, 0, 0), color);
	Debug::DrawLine(position + btVector3(0, 0, size), position + btVector3(0, 0, -size), color);
}


void Debug::Draw(const Shader& shader, const Shader& shaderUi)
{
	DrawLines(shader);
	DrawHud(shaderUi);
}

void Debug::DrawHud(const Shader& shader)
{
	shader.use();

	std::vector<Vertex> lines = {
		{ {-0.01f,0.f,0.f },{1.f,1.f,1.f } },
		{ {0.01f,0.f,0.f },{1.f,1.f,1.f } },
		{ {0.f,-0.01f,0.f },{ 1.f,1.f,1.f }},
		{ {0.f, 0.01f,0.f },{ 1.f,1.f,1.f } }
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(Vertex), lines.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, lines.size());
}

void Debug::DrawLines(const Shader& shader)
{
	shader.use();

	if (m_lines.size() > 0)
	{
		unsigned int VBO, VAO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, m_lines.size() * sizeof(Vertex), m_lines.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, color)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, m_lines.size());
	}
}

Debug::~Debug() {}