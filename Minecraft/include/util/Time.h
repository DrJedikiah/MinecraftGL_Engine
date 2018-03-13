#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Time
{
public:
	static float DeltaTime();
	static float FixedDeltaTime();
	static float ElapsedSinceStartup();

	static void SetDeltaTime( float delta );
	static void SetFixedDeltaTime( float delta );
private:
	static float m_deltaTime;
	static float m_fixedDeltaTime;
};

