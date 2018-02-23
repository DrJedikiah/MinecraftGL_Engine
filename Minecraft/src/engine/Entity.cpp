#include "engine/Entity.h"

Entity::Entity(float mass, btCollisionShape * shape, btTransform transform) :
	m_shape(shape),
	m_rb(*PhysicsEngine::CreateRigidBody(mass, transform, shape))
{
}

RigidBody& Entity::rb() const { return m_rb; }