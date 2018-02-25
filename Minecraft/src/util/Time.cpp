#include "util/Time.h"

float Time::m_deltaTime = 1.f / 144.f;
float Time::m_fixedDeltaTime = 0.01f;

float Time::DeltaTime() { return m_deltaTime; }
float Time::FixedDeltaTime() { return m_fixedDeltaTime; }
float Time::ElapsedSinceStartup() { return (float)glfwGetTime(); }