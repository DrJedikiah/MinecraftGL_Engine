#pragma once

#include "engine/World.h"
#include "engine/Camera.h"

#include "util/Input.h"
#include "util/Signal.h"

class FreeCameraController
{
public:
	FreeCameraController(Camera& camera);
	void Update(float delta);

	void SetEnabled(bool state);
	bool Enabled() const;
private:
	Camera & m_camera;
	bool m_enabled;

	float m_moveSpeed = 0.2f;
	float m_mouseXspeed = 0.005f;
	float m_mouseYspeed = 0.005f;
};