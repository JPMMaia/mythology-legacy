#include "pch.h"
#include "CppUnitTest.h"
#include "GameEngine/Component/Transforms/TransformComponent.h"

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
				t1->SetLocalPosition(expectedTranslation);
				Assert::IsTrue(expectedTranslation == t1->GetLocalPosition());
			}

			// Test rotation:
			auto pi = std::acos(-1.0f);
			{
				Quaternionf expectedRotation(AngleAxisf(pi, Vector3f(0.0f, 1.0f, 0.0f)));
				t1->SetLocalRotation(expectedRotation);
				Assert::IsTrue(expectedRotation.coeffs() == (t1->GetLocalRotation().coeffs()));
			}

			// Test scale:
			{
				Vector3f expectedScale(3.0f, 1.0f, 0.5f);
				t1->SetLocalScaling(expectedScale);
				Assert::IsTrue(expectedScale == t1->GetLocalScaling());
			}
		}

		TEST_METHOD(TransformComponentTest2)
		{
			auto pi = std::acos(-1.0f);

			{
				auto t1 = std::make_shared<TransformComponent>();
				auto worldTransformBeforeAddingParent = t1->GetWorldTransform();

				// World position stays = true:
				{
					auto parent = std::make_shared<TransformComponent>();
					parent->SetLocalPosition({ 0.0f, 1.0f, 2.0f });
					parent->SetLocalRotation(
						Quaternion<float>(
							AngleAxisf(pi, Vector3f::UnitZ()) *
							AngleAxisf(pi, Vector3f::UnitY()) *
							AngleAxisf(pi, Vector3f::UnitZ())
							)
					);
					parent->SetLocalScaling({ 0.1f, 0.4f, 0.6f });
					t1->SetParent(parent, true);

					Assert::IsTrue(t1->GetWorldTransform().isApprox(worldTransformBeforeAddingParent));

					t1->UnsetParent(true);
					Assert::IsTrue(t1->GetParent().expired());
					Assert::IsTrue(t1->GetWorldTransform().isApprox(worldTransformBeforeAddingParent));
				}
			}

			// No parent, word position stays = false:
			{
				auto parent = std::make_shared<TransformComponent>();
				parent->SetLocalPosition({ 1.0f, 2.0f, 3.0f });

				auto t1 = std::make_shared<TransformComponent>();
				t1->SetLocalPosition({ 4.0f, 5.0f, 6.0f });

				t1->SetParent(parent, false);
				Assert::IsTrue(t1->GetLocalPosition() == Vector3f(4.0f, 5.0f, 6.0f));

				t1->UnsetParent(false);
				Assert::IsTrue(t1->GetLocalPosition() == Vector3f(4.0f, 5.0f, 6.0f));
			}

			// No parent, word position stays = true:
			{
				auto parent = std::make_shared<TransformComponent>();
				parent->SetLocalPosition({ 1.0f, 2.0f, 3.0f });

				auto t1 = std::make_shared<TransformComponent>();
				t1->SetWorldPosition({ 4.0f, 5.0f, 6.0f });

				t1->SetParent(parent, true);
				Assert::IsTrue(t1->GetWorldPosition() == Vector3f(4.0f, 5.0f, 6.0f));

				t1->UnsetParent(true);
				Assert::IsTrue(t1->GetWorldPosition() == Vector3f(4.0f, 5.0f, 6.0f));
			}

			// Has parent, word position stays = false:
			{
				auto otherParent = std::make_shared<TransformComponent>();
				otherParent->SetLocalPosition({ 7.0f, 8.0f, 9.0f });

				auto parent = std::make_shared<TransformComponent>();
				parent->SetLocalPosition({ 1.0f, 2.0f, 3.0f });

				auto t1 = std::make_shared<TransformComponent>();
				t1->SetLocalPosition({ 4.0f, 5.0f, 6.0f });
				
				t1->SetParent(otherParent, false);
				t1->SetParent(parent, false);
				Assert::IsTrue(t1->GetLocalPosition() == Vector3f(4.0f, 5.0f, 6.0f));

				t1->UnsetParent(false);
				Assert::IsTrue(t1->GetLocalPosition() == Vector3f(4.0f, 5.0f, 6.0f));
			}

			// Has parent, word position stays = true:
			{
				auto otherParent = std::make_shared<TransformComponent>();
				otherParent->SetLocalPosition({ 7.0f, 8.0f, 9.0f });

				auto parent = std::make_shared<TransformComponent>();
				parent->SetLocalPosition({ 1.0f, 2.0f, 3.0f });

				auto t1 = std::make_shared<TransformComponent>();
				t1->SetWorldPosition({ 4.0f, 5.0f, 6.0f });

				t1->SetParent(otherParent, true);
				t1->SetParent(parent, true);
				Assert::IsTrue(t1->GetWorldPosition() == Vector3f(4.0f, 5.0f, 6.0f));

				t1->UnsetParent(true);
				Assert::IsTrue(t1->GetWorldPosition() == Vector3f(4.0f, 5.0f, 6.0f));
			}
		}

		TEST_METHOD(TransformComponentTest3)
		{
			auto pi = std::acos(-1.0f);

			// Apply local transformations and test world position:
			{
				auto t1 = std::make_shared<TransformComponent>();
				t1->SetLocalPosition({ 1.0f, 1.0f, 0.0f });
				t1->SetLocalRotation(Quaternionf(AngleAxisf(-pi / 2.0f, Vector3f::UnitZ())));

				auto parent = std::make_shared<TransformComponent>();
				parent->SetLocalRotation(Quaternion<float>(AngleAxisf(-pi / 2.0f, Vector3f::UnitZ())));
				t1->SetParent(parent, false);

				Assert::IsTrue(t1->GetWorldPosition().isApprox(Vector3f(1.0f, -1.0f, 0.0f)));
			}

			// Apply local transformations and test world rotation:
			{
				auto t1 = std::make_shared<TransformComponent>();
				t1->SetLocalRotation(Quaternion<float>(AngleAxisf(pi, Vector3f::UnitZ())));
				
				auto parent = std::make_shared<TransformComponent>();
				parent->SetLocalRotation(Quaternion<float>(AngleAxisf(pi, Vector3f::UnitZ())));
				t1->SetParent(parent, false);

				Assert::IsTrue(t1->GetWorldRotation().isApprox(Quaternion<float>(AngleAxisf(2.0f * pi, Vector3f::UnitZ()))));
			}

			// Apply world transformations and test local position:
			{
				auto parent = std::make_shared<TransformComponent>();
				parent->SetLocalRotation(Quaternionf(AngleAxisf(-pi / 2.0f, Vector3f::UnitZ())));

				auto t1 = std::make_shared<TransformComponent>();
				t1->SetLocalPosition({ 1.0f, 1.0f, 0.0f });
				t1->SetParent(parent, false);
				Assert::IsTrue(t1->GetLocalPosition().isApprox(Vector3f(1.0f, 1.0f, 0.0f)));
				Assert::IsTrue(t1->GetWorldPosition().isApprox(Vector3f(1.0f, -1.0f, 0.0f)));

				t1->SetWorldPosition({ 0.0f, 1.0f, 0.0f });
				Assert::IsTrue(t1->GetWorldPosition().isApprox(Vector3f(0.0f, 1.0f, 0.0f)));
				Assert::IsTrue(t1->GetLocalPosition().isApprox(Vector3f(-1.0f, 0.0f, 0.0f)));
			}

			// Apply world transformations and test local rotation:
			{
				auto parent = std::make_shared<TransformComponent>();
				parent->SetLocalRotation(Quaternionf(AngleAxisf(-pi / 2.0f, Vector3f::UnitZ())));

				auto t1 = std::make_shared<TransformComponent>();
				t1->SetLocalPosition({ 1.0f, 1.0f, 0.0f });
				t1->SetParent(parent, false);

				t1->SetWorldRotation(Quaternionf(AngleAxisf(pi / 2.0f, Vector3f::UnitZ())));
				Assert::IsTrue(t1->GetWorldRotation().isApprox(Quaternionf(AngleAxisf(pi / 2.0f, Vector3f::UnitZ()))));
				Assert::IsTrue(t1->GetLocalRotation().isApprox(Quaternionf(AngleAxisf(pi, Vector3f::UnitZ()))));
				Assert::IsTrue(t1->GetWorldPosition().isApprox(Vector3f(1.0f, -1.0f, 0.0f)));
				Assert::IsTrue(t1->GetLocalPosition().isApprox(Vector3f(1.0f, 1.0f, 0.0f)));
			}

			// Test set world transform:
			{
				auto parent = std::make_shared<TransformComponent>();
				parent->SetLocalPosition(Vector3f(1.0f, 2.0f, 3.0f));
				parent->SetLocalRotation(Quaternionf(AngleAxisf(-pi / 2.0f, Vector3f::UnitZ())));

				auto t1 = std::make_shared<TransformComponent>();
				t1->SetLocalPosition({ 1.0f, 1.0f, 1.0f });
				t1->SetLocalRotation(Quaternionf(AngleAxisf(-pi / 2.0f, Vector3f::UnitZ())));
				t1->SetParent(parent, false);
				
				t1->SetWorldTransform(Affine3f::Identity());
				Assert::IsTrue(t1->GetWorldTransform().isApprox(Affine3f::Identity()));
			}
		}

		TEST_METHOD(TransformComponentTest4)
		{
			using Vector3 = TransformComponent::Vector3Type;
			const auto pi = std::acos(-1.0f);

			auto t1 = std::make_shared<TransformComponent>();

			t1->Move(Vector3({ 1.0f, 1.0f, 1.0f }), 2.0f);
			Assert::IsTrue(t1->GetLocalPosition() == Vector3({ 2.0f, 2.0f, 2.0f }));

			t1->Move(Vector3({ -1.0f, -1.0f, -1.0f }), 2.0f);
			Assert::IsTrue(t1->GetLocalPosition() == Vector3({ 0.0f, 0.0f, 0.0f }));

			t1->MoveLocalZ(1.0f);
			Assert::IsTrue(t1->GetLocalPosition() == Vector3({ 0.0f, 0.0f, 1.0f }));

			t1->MoveLocalY(-2.0f);
			Assert::IsTrue(t1->GetLocalPosition() == Vector3({ 0.0f, -2.0f, 1.0f }));

			t1->MoveLocalX(3.0f);
			Assert::IsTrue(t1->GetLocalPosition() == Vector3({ 3.0f, -2.0f, 1.0f }));

			t1->RotateLocalZ(pi / 2.0f);
			Assert::IsTrue(t1->GetLocalRotation().isApprox(Quaternionf(AngleAxisf(pi / 2.0f, Vector3::UnitZ()))));

			t1->MoveLocalX(-4.0f);
			Assert::IsTrue(t1->GetLocalPosition().isApprox(Vector3({ 3.0f, -6.0f, 1.0f })));

			t1->MoveLocalY(5.0f);
			Assert::IsTrue(t1->GetLocalPosition().isApprox(Vector3({ -2.0f, -6.0f, 1.0f })));
		}
	};
}
