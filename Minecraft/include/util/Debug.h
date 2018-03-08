#pragma once

#include "graphics/Mesh.h"

#include <glm/glm.hpp>

#include <iostream>

class Debug
{
public:
	struct Vertex
	{
		glm::vec3  vertex;
		glm::vec3  color;
	};

	static void Clear();
	static void Draw(const Shader& shader, const Shader& shaderUi);
	static void DrawLine(glm::vec3 start, glm::vec3  end, glm::vec3  color = {0,0,0});
	static void DrawCross(glm::vec3  position, float size = 2.f, glm::vec3  color = { 0,0,0 });

private:
	//Singleton pattern
	static Debug m_instance;
	Debug& operator= (const Debug&) {}
	Debug(const Debug&) {}
	Debug();
	~Debug();

	static void DrawLines(const Shader& shader);
	static void DrawHud(const Shader& shader);

	static std::vector<Vertex> m_lines;
};