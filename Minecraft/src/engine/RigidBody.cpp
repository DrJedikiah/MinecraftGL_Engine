#include "engine/RigidBody.h"

int RigidBody::m_count = 0;


RigidBody::RigidBody(const btRigidBodyConstructionInfo &constructionInfo, Tag tag) :
	btRigidBody(constructionInfo),
	m_tag(tag)
{ 
	m_id = m_count++;
	
}

Tag RigidBody::tag() const { return m_tag;  }

void RigidBody::SetTag(Tag tag) { m_tag = tag; }

void RigidBody::ActivateCollisionSignals(bool state)
{
	if(state)
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