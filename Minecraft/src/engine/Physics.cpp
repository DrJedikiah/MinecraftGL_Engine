#include "engine/Physics.h"

PhysicsEngine PhysicsEngine::m_instance = PhysicsEngine();

 btDefaultCollisionConfiguration * PhysicsEngine::collisionConfiguration;
 btCollisionDispatcher* PhysicsEngine::dispatcher;
 btBroadphaseInterface* PhysicsEngine::overlappingPairCache;
 btSequentialImpulseConstraintSolver* PhysicsEngine::solver;
 btDiscreteDynamicsWorld* PhysicsEngine::dynamicsWorld;
 btAlignedObjectArray<btCollisionShape*> PhysicsEngine::collisionShapes;

 void PhysicsEngine::StepSimulation(float timeStep)
 {
	 dynamicsWorld->stepSimulation(timeStep, 10);
 }

 RigidBody& PhysicsEngine::CreateRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape)
 {
	 bool isDynamic = (mass != 0.f);

	 btVector3 localInertia(0, 0, 0);
	 if (isDynamic)
		 shape->calculateLocalInertia(mass, localInertia);

	 btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	 btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);
	 RigidBody* body = new RigidBody(cInfo);

	 body->setUserIndex(-1);
	 dynamicsWorld->addRigidBody(body);
	 return *body;
 }

 btCollisionWorld::ClosestRayResultCallback PhysicsEngine::RayCast(btVector3 Start, btVector3 End)
 {
	 btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
	 dynamicsWorld->rayTest(Start, End, RayCallback);
	 return RayCallback;
 }

 bool PhysicsEngine::ContactAdded(btManifoldPoint& pt, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1)
 {
	 //Get the two RigidBodies
	 RigidBody* objA = (RigidBody*)colObj0Wrap->getCollisionObject();
	 RigidBody* objB = (RigidBody*)colObj1Wrap->getCollisionObject();

	 //If the contact has not been processed yet
	 if (pt.m_userPersistentData == nullptr)
	 {
		 //colA and colB are set if their CollisionSignals are Activateds
		 RigidBody::Collision * colA = nullptr;
		 RigidBody::Collision * colB = nullptr;
		 if(objA->CollisionSignalsActivateds())
			 colA = &(objA->m_collisions[objB->id()]);
		 if (objB->CollisionSignalsActivateds())
			 colB = &(objB->m_collisions[objA->id()]);

		 //Get the first RigidBody::Collision activated, returns if none is
		 RigidBody::Collision * col = nullptr;
		 if (colA)
			 col = colA;
		 else
			 col = colB;
		 if ( ! col )
			 return false;

		 //Set the  RigidBody::Collision data and the point PersistentData for Destruction lk
		pt.m_userPersistentData = (void*)col;
		++col->numContacts;
		col->rb1 = objA;
		col->rb2 = objB;

		//Send signals if it is the first contact
		 if (col->numContacts == 1)
		 {
			if(colA)
				objA->onCollisionEnter.emmit(*objB, pt);
			if (colB)
				objB->onCollisionEnter.emmit(*objA, pt);
		 } 
	 }
	 return false;
 }

 bool PhysicsEngine::ContactDestroyed(void* userPersistentData)
 {

	 //Get the RigidBody::Collision data
	 RigidBody::Collision * col = ((RigidBody::Collision*)userPersistentData);
	 --col->numContacts;

	 //Send signals if there is no more contact
	 if (col->numContacts == 0)
	 {
		 if (col->rb1->CollisionSignalsActivateds())
			 col->rb1->onCollisionExit.emmit(*col->rb2);
		 if (col->rb2->CollisionSignalsActivateds())
			 col->rb2->onCollisionExit.emmit(*col->rb1);
	 }

	 return false;
 }


PhysicsEngine::PhysicsEngine()
{
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -20, 0));

	gContactAddedCallback = ContactAdded;
	gContactDestroyedCallback = ContactDestroyed;
}

PhysicsEngine::~PhysicsEngine()
{
	//cleanup in the reverse order of creation/initialization
	//remove the rigidbodies from the dynamics world and delete them
	/*for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

	//delete collision shapes
	for (int j = 0; j < collisionShapes.size(); j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		collisionShapes[j] = 0;
		delete shape;
	}

	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();*/
}


