#include "pch.h"
#include "CppUnitTest.h"
#include "GameEngine/GameObject/GameObject.h"
#include "GameEngine/Physics/PhysicsManager.h"
#include "GameEngine/Component/Physics/PhysicsComponent.h"

using namespace physx;
using namespace GameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MythologyTestProject
{
	TEST_CLASS(PhysicsComponentTests)
	{
	private:
		bool AreEqual(const PxTransform& pxTransform, const Eigen::Affine3f& eigenTransform)
		{
			auto pxMatrix = PxMat44(pxTransform);
			auto eigenMatrix = eigenTransform.matrix();

			for (auto i = 0; i < 4; ++i)
			{
				for (auto j = 0; j < 4; ++j)
				{
					if (pxMatrix(i, j) != eigenMatrix(i, j))
						return false;
				}
			}

			return true;
		}

	public:
		TEST_METHOD(PhysicsComponentTest1)
		{
			PhysicsManager manager;
			const auto& physics = manager.GetPhysics();
			auto scene = manager.CreateScene();

			// Create shape and material:
			auto material = physics->createMaterial(0.5f, 0.5f, 0.6f);
			auto halfExtent = 1.0f;
			auto shape = PhysXUniquePointer<PxShape>(physics->createShape(PxBoxGeometry(halfExtent, halfExtent, halfExtent), *material));

			// Create rigid dynamic:
			PxTransform transform(PxVec3(0.0f, 0.0f, 0.0f));
			auto body = physics->createRigidDynamic(transform);
			body->attachShape(*shape);
			PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
			body->addForce(PxVec3(0.0f, 1.0f, 0.0f));
			scene->addActor(*body);

			// TODO associate physics component with rigid actor:
			auto physicsComponent = PhysicsComponent::CreateSharedPointer();

			// Create game object and add physics component:
			GameObject object;
			object.AddComponent("Physics", physicsComponent);
			
			// Ensure that both game object and rigid body transforms are the identity:
			Assert::IsTrue(body->getGlobalPose() == PxTransform(PxVec3(0.0f, 0.0f, 0.0f)));
			Assert::IsTrue(object.GetTransform().GetWorldTransform().isApprox(Eigen::Affine3f::Identity()));

			// Simulate physics:
			scene->simulate(1.0f / 60.0f);
			scene->fetchResults(true);

			// Check if both game object and rigid body transforms changed:
			Assert::IsFalse(body->getGlobalPose() == PxTransform(PxVec3(0.0f, 0.0f, 0.0f)));
			Assert::IsFalse(object.GetTransform().GetWorldTransform().isApprox(Eigen::Affine3f::Identity()));

			// Check that both game object and rigid transforms are equal:
			Assert::IsTrue(AreEqual(body->getGlobalPose(), object.GetTransform().GetWorldTransform()));
		}
	};
}
