#include "engine/PlayerController.h"

PlayerController::PlayerController(Camera& camera, PlayerAvatar& avatar) :
	m_camera(camera),
	m_avatar(avatar)
{
	m_avatar.rb().setFriction(0.f);

	m_avatar.rb().onCollisionEnter.connect(&PlayerController::OnCollisionEnter, this);
	m_avatar.rb().onCollisionExit.connect(&PlayerController::OnCollisionExit, this);

	m_avatar.rb().ActivateCollisionSignals(true);
	m_avatar.rb().setActivationState(DISABLE_DEACTIVATION);

	m_avatar.rb().setFriction(0.f);
}



void PlayerController::Update(float delta)
{
	if (m_enabled)
	{
		MoveAvatar();
		SetCamera();

		/*if (Mouse::ButtonPressed(Mouse::Button::left))
		{
			
			btVector3 start = bt::toVec3(m_camera.position());
			btVector3 direction = bt::toVec3(m_camera.forward());
			btCollisionWorld::ClosestRayResultCallback res = PhysicsEngine::RayCast(start, start + 2.f * direction);
			if (res.hasHit())
			{
				glm::ivec3 blockCoord = glm::toVec3(res.m_hitPointWorld);// -Block::size / 2 * res.m_hitNormalWorld);
				
				if (Mouse::ButtonPressed(Mouse::Button::left))
					World::RemoveBlock(blockCoord);
			


				start = bt::toVec3(m_camera.position());
				Debug::DrawCross(bt::toVec3(blockCoord), { 1,0,0 });
				
			}
			
			Debug::DrawLine(start, start + direction);
		}	*/

	} 
	btVector3 start = bt::toVec3(m_camera.position());
	btVector3 direction = bt::toVec3( 3.f * m_camera.forward());

	Debug::DrawLine(start, start + direction);
	btCollisionWorld::ClosestRayResultCallback res = PhysicsEngine::RayCast(start, start + direction);
	if (res.hasHit())
	{
		glm::ivec3 blockCoord = glm::toVec3(res.m_hitPointWorld + btVector3(0.5f, 0.5f, 0.5f) - Block::size / 2 * res.m_hitNormalWorld);
		Debug::DrawCross(res.m_hitPointWorld - Block::size / 2  *res.m_hitNormalWorld, 0.5f, { 1,0,0 });
		Debug::DrawCross({ (float)blockCoord.x, (float)blockCoord.y, (float)blockCoord.z }, 2.f , { 0,1,0 });

		if (Mouse::ButtonPressed(Mouse::Button::left))
		{
			std::cout << res.m_hitPointWorld.getX() << "\t" << res.m_hitPointWorld.getZ() << " ###" << blockCoord.x << " " << blockCoord.z << std::endl;
			World::RemoveBlock(blockCoord);
		}

	}
	//Debug::DrawCross(, { 1,0,0 });

} 

void PlayerController::MoveAvatar()
{
	//Run
	if (Keyboard::KeyPressed(GLFW_KEY_LEFT_SHIFT))
		m_isRunning = true;
	if (Keyboard::KeyReleased(GLFW_KEY_LEFT_SHIFT))
		m_isRunning = false;

	//Set a movemennt direction vector
	btVector3 direction(0, 0, 0);
	if (Keyboard::KeyDown(Keyboard::AzertyKey::Z))
		direction += btVector3(m_camera.forward().x, 0, m_camera.forward().z).normalized();
	if (Keyboard::KeyDown(Keyboard::AzertyKey::S))
		direction -= btVector3(m_camera.forward().x, 0, m_camera.forward().z).normalized();
	if (Keyboard::KeyDown(Keyboard::AzertyKey::D))
		direction += btVector3(m_camera.right().x, 0, m_camera.right().z).normalized();
	if (Keyboard::KeyDown(Keyboard::AzertyKey::Q))
		direction -= btVector3(m_camera.right().x, 0, m_camera.right().z).normalized();


	if (direction.isZero())
	{
		//Stops the player whe not trying to move
		if (!m_isJumping)
			m_avatar.rb().setLinearVelocity(btVector3(0, m_avatar.rb().getLinearVelocity().getY(), 0));
	}
	else
	{
		btVector3 velocity = m_avatar.rb().getLinearVelocity();
		velocity.setY(0);
		if (!velocity.isZero())
		{	
			//Stop if the player want's to go in the reverse direction
			if (velocity.dot(direction) < 0)
				m_avatar.rb().setLinearVelocity(btVector3(0, m_avatar.rb().getLinearVelocity().getY(), 0));

			//Slow down if too fast
			float maxSpeed = m_isRunning ? m_runSpeed : m_walkSpeed;
			if (velocity.norm() > maxSpeed)
			{
				velocity = maxSpeed * velocity.normalized();
				velocity.setY(m_avatar.rb().getLinearVelocity().getY());
				m_avatar.rb().setLinearVelocity(velocity);
			}
		}  

		//Increase speed 
		if(m_isRunning)
			m_avatar.rb().applyCentralForce(m_accelerationRun * direction);
		else
			m_avatar.rb().applyCentralForce(m_accelerationWalk * direction);
	}

	//Jump
	if (Keyboard::KeyPressed(GLFW_KEY_SPACE) && isHittingFloor())
	{
		m_isJumping = true;
		m_avatar.rb().applyCentralImpulse(btVector3(0, m_jumpStrenght, 0));
	}
}

void PlayerController::SetCamera()
{
	//Camera
	m_camera.SetPosition(glm::toVec3(m_avatar.rb().transform().getOrigin()) + glm::vec3(0, 3.f * m_avatar.height / 4.f, 0));
	m_camera.RotateRight(m_mouseXspeed * Mouse::delta().x);
	m_camera.RotateUp(-m_mouseYspeed * Mouse::delta().y);
}

///<summary>Returns true if the player is touching the ground (performs raycasts)</summary>
bool PlayerController::isHittingFloor()
{
	//Raycast 4 corners near the player foot
	btVector3 Direction = m_avatar.height *  btVector3(0, -1, 0);
	btVector3 Start1 = m_avatar.rb().transform().getOrigin() + btVector3(m_avatar.radius, 0, m_avatar.radius);
	btVector3 Start2 = m_avatar.rb().transform().getOrigin() + btVector3(m_avatar.radius, 0, -m_avatar.radius);
	btVector3 Start3 = m_avatar.rb().transform().getOrigin() + btVector3(-m_avatar.radius, 0, m_avatar.radius);
	btVector3 Start4 = m_avatar.rb().transform().getOrigin() + btVector3(-m_avatar.radius, 0, -m_avatar.radius);

	if (PhysicsEngine::RayCast(Start1, Start1 + Direction).hasHit()) return true;
	if (PhysicsEngine::RayCast(Start2, Start2 + Direction).hasHit()) return true;
	if (PhysicsEngine::RayCast(Start3, Start3 + Direction).hasHit()) return true;
	if (PhysicsEngine::RayCast(Start4, Start4 + Direction).hasHit()) return true;


	return false;
}

void PlayerController::OnCollisionEnter(RigidBody& other, btManifoldPoint& point)
{
	if (isHittingFloor())
		m_isJumping = false;
}

void PlayerController::OnCollisionExit(RigidBody& other)
{
	if ( ! isHittingFloor())
		m_isJumping = true;
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


