#pragma once

#include "btBulletDynamicsCommon.h"

#include <map>

#include "util/Signal.h"
#include "util/Util.h"


#include <glm/gtc/type_ptr.hpp>


class RigidBody : public btRigidBody
{
public:
	friend class Physics;

	RigidBody(const btRigidBodyConstructionInfo &constructionInfo, Tag tag = Tag::def);

	Tag tag() const;
	void SetTag(Tag tag);

	glm::vec3 Position() const;
	glm::quat Rotation() const;

	//void SetPosition(glm::vec3 position);

	void ActivateCollisionSignals(bool state);
	bool CollisionSignalsActivateds();

	Signal<RigidBody&, btManifoldPoint&> onCollisionEnter;
	Signal<RigidBody&> onCollisionExit;

	int id() const;

private:
	Tag m_tag;

	static int m_count;
	int m_id;
	int m_index;

	struct Collision
	{
		RigidBody * rb1;
		RigidBody * rb2;
		int numContacts;
	};
	std::map<unsigned, Collision> m_collisions;
};