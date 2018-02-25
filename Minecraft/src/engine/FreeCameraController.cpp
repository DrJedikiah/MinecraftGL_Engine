#include "engine/FreeCameraController.h"

FreeCameraController::FreeCameraController(Camera& camera) :
	m_camera(camera)
{
}

void FreeCameraController::Update(float delta)
{
	if (m_enabled)
	{
		float speed = m_moveSpeed;
		if (Keyboard::KeyDown(GLFW_KEY_LEFT_SHIFT))
			speed = m_boostMoveSpeed;

		if (Keyboard::KeyDown(Keyboard::AzertyKey::Z))
			m_camera.Translate(speed * m_camera.forward());
		if (Keyboard::KeyDown(Keyboard::AzertyKey::S))
			m_camera.Translate(-speed * m_camera.forward());
		if (Keyboard::KeyDown(Keyboard::AzertyKey::Q))
			m_camera.Translate(-speed * m_camera.right());
		if (Keyboard::KeyDown(Keyboard::AzertyKey::D))
			m_camera.Translate(speed * m_camera.right());
		if (Keyboard::KeyDown(Keyboard::AzertyKey::A))
			m_camera.Translate(-speed * m_camera.up());
		if (Keyboard::KeyDown(Keyboard::AzertyKey::E))
			m_camera.Translate(speed * m_camera.up());

		if (Mouse::KeyDown(GLFW_MOUSE_BUTTON_RIGHT))
		{
			m_camera.RotateRight(m_mouseXspeed * Mouse::delta().x);
			m_camera.RotateUp(-m_mouseYspeed * Mouse::delta().y);

			glm::ivec2 size = Input::GetWindowSize();
			Mouse::LockCursor(true);
		}
		else
			Mouse::LockCursor(false);
	}
}

bool FreeCameraController::Enabled() const { return m_enabled; }
void FreeCameraController::SetEnabled(bool state)
{
	if (m_enabled != state)
	{
		m_enabled = state;
		if (m_enabled)
		{
			Mouse::SetCursor(Mouse::CursorState::normal);
			Mouse::LockCursor(false);
		}
	}
}