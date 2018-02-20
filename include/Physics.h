#pragma once

#include "btBulletDynamicsCommon.h"
#include <iostream>

class PhysicsEngine
{
public:
	PhysicsEngine();
	~PhysicsEngine();

	void StepSimulation( float timeStep );
	btRigidBody* CreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape);
private:

	btDefaultCollisionConfiguration * collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingPairCache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	btAlignedObjectArray<btCollisionShape*> collisionShapes;
};