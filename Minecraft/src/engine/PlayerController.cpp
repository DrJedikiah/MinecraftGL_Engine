#include "engine/PlayerController.h"

PlayerController::PlayerController(Camera& camera, PlayerAvatar& avatar) :
	m_camera(camera),
	m_avatar(avatar)
{
	m_avatar.rb().setFriction(0.f);

	m_avatar.rb().onCollisionEnter.connect(&PlayerController::OnCollisionEnter, this);
	m_avatar.rb().onCollisionExit.connect(&PlayerController::OnCollisionExit, this);

	m_avatar.rb().ActivateCollisionSignals(true);
}

void PlayerController::Update(float delta)
{
	if (m_enabled)
	{
		//Move
		btVector3 direction(0, 0, 0);
		if (Keyboard::KeyDown(Keyboard::AzertyKey::Z))
			direction += btVector3(m_camera.forward().x, 0, m_camera.forward().z).normalized();
		if (Keyboard::KeyDown(Keyboard::AzertyKey::S))
			direction -= btVector3(m_camera.forward().x, 0, m_camera.forward().z).normalized();
		if (Keyboard::KeyDown(Keyboard::AzertyKey::D))
			direction += btVector3(m_camera.right().x, 0, m_camera.right().z).normalized();
		if (Keyboard::KeyDown(Keyboard::AzertyKey::Q))
			direction -= btVector3(m_camera.right().x, 0, m_camera.right().z).normalized();
		
		if (direction.norm() <= 0.f)
		{
			direction.setY(m_avatar.rb().getLinearVelocity().getY());
			m_avatar.rb().setLinearVelocity(direction);
		}
		else
		{
			btVector3 vel = m_avatar.rb().getLinearVelocity();
			vel.setY(0.f);
			direction =  mathf::clamp(vel.norm() + 0.1f, 0.f, 3.f) * direction.normalized();
			direction.setY(m_avatar.rb().getLinearVelocity().getY());
			m_avatar.rb().setLinearVelocity(direction);
		}

		//Jump
		if (Keyboard::KeyPressed(GLFW_KEY_SPACE))
			m_avatar.rb().applyImpulse(btVector3(0,5, 0), btVector3(0, 0, 0));

		//Camera
		m_camera.SetPosition(glm::toVec3(m_avatar.transform().getOrigin()) + glm::vec3(0,1.f, 0));
		m_camera.RotateRight(m_mouseXspeed * Mouse::delta().x);
		m_camera.RotateUp(-m_mouseYspeed * Mouse::delta().y);

		if (Mouse::KeyDown(GLFW_MOUSE_BUTTON_LEFT))
		{
			/*btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);

			// Perform raycast
			World->rayTest(Start, End, RayCallback);

			if (RayCallback.hasHit()) {
			End = RayCallback.m_hitPointWorld;
			Normal = RayCallback.m_hitNormalWorld;*/
		}
	}
}

void PlayerController::OnCollisionEnter(RigidBody& other, btManifoldPoint& point)
{
		std::cout << "OnCollisionEnter" << std::endl;
}

void PlayerController::OnCollisionExit(RigidBody& other)
{
		std::cout << "OnCollisionExit" << std::endl;
}

bool PlayerController::Enabled() const { return m_enabled; }
void PlayerController::SetEnabled(bool state)
{
	if (m_enabled != state)
	{
		m_enabled = state;
		if (m_enabled)
		{
			Mouse::SetCursor(Mouse::CursorState::hidden);
			Mouse::CenterCursor(true);
		}
	}
}


