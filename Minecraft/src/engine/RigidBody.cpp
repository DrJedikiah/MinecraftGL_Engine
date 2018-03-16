#include "engine/RigidBody.h"

int RigidBody::m_count = 0;


RigidBody::RigidBody(const btRigidBodyConstructionInfo &constructionInfo, Tag tag) :

	btRigidBody(constructionInfo),
	m_tag(tag)
{
	m_id = m_count++;

}

Tag RigidBody::tag() const { return m_tag; }
void RigidBody::SetTag(Tag tag) { m_tag = tag; }

glm::vec3 RigidBody::Position() const
{
	btTransform trans;
	getMotionState()->getWorldTransform(trans);
	btVector3 pos = trans.getOrigin();

	return glm::vec3(pos.getX(), pos.getY(), pos.getZ());
}

glm::quat RigidBody::Rotation() const
{
	btTransform trans;
	getMotionState()->getWorldTransform(trans);
	btQuaternion quat = trans.getRotation();

	return glm::quat(quat.getW(), quat.getX(), quat.getY(), quat.getZ());
}

/*void RigidBody::SetPosition(glm::vec3 position)
{
	btTransform trans;
	getMotionState()->getWorldTransform(trans);
	trans.setOrigin(btVector3(position.x, position.y, position.z));
	getMotionState()->setWorldTransform(trans);
}*/

void RigidBody::ActivateCollisionSignals(bool state)
{
	if (state)
		setCollisionFlags(getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	else
		setCollisionFlags(getCollisionFlags() & ~btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
}

bool RigidBody::CollisionSignalsActivateds()
{
	return getCollisionFlags() && btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK;
}

int RigidBody::id() const
{
	return m_id;
}