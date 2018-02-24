#pragma once

#include<algorithm>

#include "engine/World.h"
#include "engine/Camera.h"
#include "engine/PlayerAvatar.h"

#include "util/Input.h"
#include "util/Signal.h"
#include "util/Debug.h"


inline glm::vec3 a( btVector3 vec )
{

};


class PlayerController
{
public:
	PlayerController(Camera& camera, PlayerAvatar& avatar);
	void Update(float delta);

	void SetEnabled(bool state);
	bool Enabled() const;

	const float jumpStrength = 1000.f;
	const float moveSpeed = 3;

private:
	void MoveAvatar();
	void SetCamera();

	bool SelectBlock(glm::ivec3 & blockCoord);
	bool SelectSpace(glm::ivec3 & blockCoord);

	Camera & m_camera;
	PlayerAvatar& m_avatar;

	bool isHittingFloor();
	void OnCollisionEnter(RigidBody& other, btManifoldPoint& point);
	void OnCollisionExit(RigidBody& other);

	const float m_jumpStrenght = 8.4f;
	const float m_walkSpeed = 3.f;
	const float m_runSpeed = 6.f;
	const float m_accelerationWalk = 15.f;
	const float m_accelerationRun = 30.f;
	const float m_range = 4.f;

	float m_mouseXspeed = 0.005f;
	float m_mouseYspeed = 0.005f;

	bool m_isRunning = false;
	bool m_isJumping = false;

	bool m_enabled;
};