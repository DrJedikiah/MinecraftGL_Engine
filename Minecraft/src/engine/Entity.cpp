#include "engine/Entity.h"

Entity::Entity(float mass, btCollisionShape * shape, btTransform transform) :
	m_shape(shape),
	m_rb(PhysicsEngine::CreateRigidBody(mass, transform, shape))
{

}

btTransform Entity::transform() const
{
	btTransform trans;
	rb().getMotionState()->getWorldTransform(trans);
	return trans;
}

RigidBody& Entity::rb() const { return m_rb; }