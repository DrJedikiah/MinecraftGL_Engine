#pragma once

#include "btBulletDynamicsCommon.h"
#include <iostream>

class PhysicsEngine
{
public:
	static void StepSimulation( float timeStep );
	static btRigidBody* CreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape);

private:
	static PhysicsEngine m_instance;

	PhysicsEngine& operator= (const PhysicsEngine&) {}
	PhysicsEngine(const PhysicsEngine&) {}
	PhysicsEngine();
	~PhysicsEngine();

	 static btDefaultCollisionConfiguration * collisionConfiguration;
	 static btCollisionDispatcher* dispatcher;
	 static btBroadphaseInterface* overlappingPairCache;
	 static btSequentialImpulseConstraintSolver* solver;
	 static btDiscreteDynamicsWorld* dynamicsWorld;

	 static btAlignedObjectArray<btCollisionShape*> collisionShapes;
};