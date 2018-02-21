#pragma once

#include "engine/Physics.h"
#include "graphics/Drawable.h"
#include "graphics/Model.h"

class Entity : public Drawable
{
public:
	Entity(float mass, btCollisionShape * shape, btTransform transform = btTransform::getIdentity());

	btRigidBody & rb();
	btTransform transform();

	virtual void Update(float delta) = 0;
	virtual void UpdateModels() = 0;

private:
	btRigidBody * m_rb;
	btCollisionShape * m_shape;

};