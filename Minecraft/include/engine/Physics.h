#pragma once


#include <iostream>
#include <vector>

#include "engine/RigidBody.h"
#include "util/Input.h"

#include "btBulletDynamicsCommon.h"


class PhysicsEngine
{
public:
	static void StepSimulation( float timeStep );
	static RigidBody* CreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape);
	static bool DeleteRigidBody(RigidBody* rigidBody);
	static btCollisionWorld::ClosestRayResultCallback RayCast(btVector3 Start, btVector3 End);

private:
	//Singleton pattern
	static PhysicsEngine m_instance;
	PhysicsEngine& operator= (const PhysicsEngine&) {}
	PhysicsEngine(const PhysicsEngine&) {}
	PhysicsEngine();
	~PhysicsEngine();

	//Usefull members to create a bullet physics engine
	 static btDefaultCollisionConfiguration * collisionConfiguration;
	 static btCollisionDispatcher* dispatcher;
	 static btBroadphaseInterface* overlappingPairCache;
	 static btSequentialImpulseConstraintSolver* solver;
	 static btDiscreteDynamicsWorld* dynamicsWorld;

	 //Callbacks for the physics collisions between rigidbodies
	 static bool ContactDestroyed(void* userPersistentData);
	 static bool ContactAdded(btManifoldPoint& pt, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1);
};

