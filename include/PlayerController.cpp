#include "PlayerController.h"

PlayerController::PlayerController(Camera& camera) :
	m_camera(camera)
{
}

void PlayerController::Update(float delta)
{
	if (Keyboard::IsKeyDown( AzertyKey::Z ) )
		m_camera.translate(m_moveSpeed * m_camera.Forward() );
	if (Keyboard::IsKeyDown( AzertyKey::S))
		m_camera.translate( -m_moveSpeed * m_camera.Forward());
	if (Keyboard::IsKeyDown( AzertyKey::Q))
		m_camera.translate(-m_moveSpeed * m_camera.Right());
	if (Keyboard::IsKeyDown( AzertyKey::D))
		m_camera.translate(m_moveSpeed * m_camera.Right());
	if (Keyboard::IsKeyDown(AzertyKey::A))
		m_camera.translate(-m_moveSpeed * m_camera.Up());
	if (Keyboard::IsKeyDown(AzertyKey::E))
		m_camera.translate(m_moveSpeed * m_camera.Up());

	if ( Mouse::KeyDown( GLFW_MOUSE_BUTTON_LEFT) )
	{
		m_camera.rotateRight(m_mouseXspeed * Mouse::Delta().x);
		m_camera.rotateUp(-m_mouseYspeed * Mouse::Delta().y);
	}

}

