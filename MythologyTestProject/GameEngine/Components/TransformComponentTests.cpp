#include "stdafx.h"
#include "CppUnitTest.h"
#include "GameEngine/Component/Transforms/TransformComponent.h"
#include "Common/Timer.h"

#include <cmath>

using namespace Eigen;
using namespace GameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MythologyTestProject
{
	TEST_CLASS(TransformComponentTests)
	{
	public:
		TEST_METHOD(TransformComponentTest1)
		{
			auto t1 = std::make_shared<TransformComponent>();

			// Test translation:
			{
				Vector3f expectedTranslation(1.0f, -1.0f, 2.0f);
				t1->SetLocalTranslation(expectedTranslation);
				Assert::IsTrue(expectedTranslation == t1->LocalTranslation());
			}

			// Test rotation:
			auto pi = std::acos(-1.0f);
			{
				Quaternionf expectedRotation(AngleAxisf(pi, Vector3f(0.0f, 1.0f, 0.0f)));
				t1->SetLocalRotation(expectedRotation);
				Assert::IsTrue(expectedRotation.coeffs() == (t1->LocalRotation().coeffs()));
			}

			// Test scale:
			{
				Vector3f expectedScale(3.0f, 1.0f, 0.5f);
				t1->SetLocalScaling(expectedScale);
				Assert::IsTrue(expectedScale == t1->LocalScaling());
			}

			// Test setting parent:
			{
				Common::Timer timer(std::chrono::milliseconds(12));
				t1->FixedUpdate(timer);
				auto worldTransformBeforeAddingParent = t1->WorldTransform();

				// World position stays = true:
				{
					auto parent = std::make_shared<TransformComponent>();
					parent->SetLocalTranslation({ 0.0f, 1.0f, 2.0f });
					parent->SetLocalRotation(
						Quaternion<float>(
							AngleAxisf(pi, Vector3f::UnitZ()) *
							AngleAxisf(pi, Vector3f::UnitY()) *
							AngleAxisf(pi, Vector3f::UnitZ())
							)
					);
					parent->SetLocalScaling({ 0.1f, 0.4f, 0.6f });
					t1->SetParent(parent, true);

					t1->FixedUpdate(timer);

					Assert::IsTrue(t1->WorldTransform().isApprox(worldTransformBeforeAddingParent));
				}
			}
		}

		TEST_METHOD(TransformComponentTest2)
		{
			auto pi = std::acos(-1.0f);

			// Apply local transformations and test world position:
			{
				auto t1 = std::make_shared<TransformComponent>();
				t1->SetLocalTranslation({ 1.0f, 1.0f, 0.0f });
				t1->SetLocalRotation(Quaternionf(AngleAxisf(-pi / 2.0f, Vector3f::UnitZ())));

				auto parent = std::make_shared<TransformComponent>();
				parent->SetLocalRotation(Quaternion<float>(AngleAxisf(-pi / 2.0f, Vector3f::UnitZ())));
				t1->SetParent(parent, false);

				Assert::IsTrue(t1->WorldPosition().isApprox(Vector3f(1.0f, -1.0f, 0.0f)));
			}

			// Apply local transformations and test world rotation:
			{
				auto t1 = std::make_shared<TransformComponent>();
				t1->SetLocalRotation(Quaternion<float>(AngleAxisf(pi, Vector3f::UnitZ())));
				
				auto parent = std::make_shared<TransformComponent>();
				parent->SetLocalRotation(Quaternion<float>(AngleAxisf(pi, Vector3f::UnitZ())));
				t1->SetParent(parent, false);

				Assert::IsTrue(t1->WorldRotation().isApprox(Quaternion<float>(AngleAxisf(2.0f * pi, Vector3f::UnitZ()))));
			}

			// Apply world transformations and test local position:
			{
				auto parent = std::make_shared<TransformComponent>();
				parent->SetLocalRotation(Quaternionf(AngleAxisf(-pi / 2.0f, Vector3f::UnitZ())));

				auto t1 = std::make_shared<TransformComponent>();
				t1->SetLocalTranslation({ 1.0f, 1.0f, 0.0f });
				t1->SetParent(parent, false);
				Assert::IsTrue(t1->LocalTranslation().isApprox(Vector3f(1.0f, 1.0f, 0.0f)));
				Assert::IsTrue(t1->WorldPosition().isApprox(Vector3f(1.0f, -1.0f, 0.0f)));

				t1->SetWorldPosition({ 0.0f, 1.0f, 0.0f });
				Assert::IsTrue(t1->WorldPosition().isApprox(Vector3f(0.0f, 1.0f, 0.0f)));
				Assert::IsTrue(t1->LocalTranslation().isApprox(Vector3f(-1.0f, 0.0f, 0.0f)));
			}

			// Apply world transformations and test local rotation:
			{
				auto parent = std::make_shared<TransformComponent>();
				parent->SetLocalRotation(Quaternionf(AngleAxisf(-pi / 2.0f, Vector3f::UnitZ())));

				auto t1 = std::make_shared<TransformComponent>();
				t1->SetLocalTranslation({ 1.0f, 1.0f, 0.0f });
				t1->SetParent(parent, false);

				t1->SetWorldRotation(Quaternionf(AngleAxisf(pi / 2.0f, Vector3f::UnitZ())));
				Assert::IsTrue(t1->WorldRotation().isApprox(Quaternionf(AngleAxisf(pi / 2.0f, Vector3f::UnitZ()))));
				Assert::IsTrue(t1->LocalRotation().isApprox(Quaternionf(AngleAxisf(pi, Vector3f::UnitZ()))));
				Assert::IsTrue(t1->WorldPosition().isApprox(Vector3f(1.0f, -1.0f, 0.0f)));
				Assert::IsTrue(t1->LocalTranslation().isApprox(Vector3f(1.0f, 1.0f, 0.0f)));
			}
		}
	};
}
