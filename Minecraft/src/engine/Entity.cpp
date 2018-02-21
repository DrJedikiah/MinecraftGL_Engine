#include "engine/Entity.h"

Entity::Entity(float mass, btCollisionShape * shape, btTransform transform) :
	m_shape(shape)
{
	m_rb = PhysicsEngine::CreateRigidBody(mass, transform, shape);
}

btTransform Entity::transform()
{
	btTransform trans;
	rb().getMotionState()->getWorldTransform(trans);
	return trans;
}

btRigidBody& Entity::rb() { return *m_rb; }