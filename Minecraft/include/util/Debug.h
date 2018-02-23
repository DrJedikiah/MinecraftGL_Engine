#pragma once

#include "graphics/Mesh.h"
#include "btBulletDynamicsCommon.h"

#include <iostream>

class Debug
{
public:
	struct Vertex
	{
		btVector3 vertex;
		btVector3 color;
	};

	static void Clear();
	static void Draw(const Shader& shader, const Shader& shaderUi);
	static void DrawLine(btVector3 start, btVector3 end, btVector3 color = {0,0,0});
	static void DrawCross(btVector3 position, float size = 2.f, btVector3 color = { 0,0,0 });

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