#pragma once

#include "graphics/Mesh.h"

#include <glm/glm.hpp>
#include <map>

#include <iostream>


//Every object that will draw debug better implement the IWithDebug interface
//It will be added to the list of IWithDebug and OnDrawDebug will be called only when necessary
class IWithDebug
{
public:
	friend class Debug;

	IWithDebug();
	~IWithDebug();

private:
	virtual void OnDrawDebug() const = 0;

	int m_id;

	static int m_count;
	static std::map<int, IWithDebug*> * m_IWithDebugInstances;

};

class Debug
{
public:
	struct Vertex
	{
		glm::vec3  vertex;
		glm::vec4  color;
	};

	static void Clear();
	static void Draw(const Shader& shader, const Shader& shaderUi);
	static void DrawLine(glm::vec3 start, glm::vec3  end, glm::vec4  color = {0,0,0,0.5f});
	static void DrawCross(glm::vec3  position, float size = 2.f, glm::vec4  color = { 0,0,0,0.5f });

	static void SetActivated( bool state );
	static bool Activated();


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

	static bool m_activated;
};