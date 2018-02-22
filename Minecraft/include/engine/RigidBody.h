#pragma once

#include "btBulletDynamicsCommon.h"

#include <map>

#include "util/Signal.h"
#include "util/Util.h"

class RigidBody : public btRigidBody
{
public:
	friend class PhysicsEngine;

	RigidBody(const btRigidBodyConstructionInfo &constructionInfo, Tag tag = Tag::default);

	Tag tag() const;
	void SetTag(Tag tag);

	void ActivateCollisionSignals(bool state);
	bool CollisionSignalsActivateds();

	Signal<RigidBody&, btManifoldPoint&> onCollisionEnter;
	Signal<RigidBody&, btManifoldPoint&> onCollisionStay;
	Signal<RigidBody&> onCollisionExit;

	int id() const;

private:
	Tag m_tag;

	static int m_count;
	int m_id;

	struct Collision
	{
		RigidBody * rb1;
		RigidBody * rb2;
		int numContacts;
	};
	std::map<unsigned, Collision> m_collisions;
};