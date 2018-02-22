#pragma once

#include<algorithm>

#include "engine/World.h"
#include "engine/Camera.h"
#include "engine/PlayerAvatar.h"

#include "util/Input.h"
#include "util/Signal.h"

class PlayerController
{
public:
	PlayerController(Camera& camera, PlayerAvatar& avatar);
	void Update(float delta);

	void SetEnabled(bool state);
	bool Enabled() const;

	const float jumpStrength = 100.f;
	const float moveSpeed = 3;

private:
	Camera & m_camera;
	PlayerAvatar& m_avatar;

	void OnCollisionEnter(RigidBody& other, btManifoldPoint& point);
	void OnCollisionExit(RigidBody& other);

	bool m_enabled;

	float m_moveSpeed = 0.2f;
	float m_mouseXspeed = 0.005f;
	float m_mouseYspeed = 0.005f;


};