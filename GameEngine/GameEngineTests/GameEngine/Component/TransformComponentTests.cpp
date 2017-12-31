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
	private:
		using Vector3 = TransformComponent::Vector3Type;
		using Quaternion = TransformComponent::QuaternionType;

		const float c_pi = std::acos(-1.0f);

	public:
		TEST_METHOD(DefaultConstructor)
		{
			auto transform = std::make_shared<TransformComponent>();
			Assert::IsTrue(Vector3(0.0f, 0.0f, 0.0f) == transform->GetLocalPosition());
			Assert::IsTrue(Quaternion::Identity().isApprox(transform->GetLocalRotation()));
			Assert::IsTrue(Vector3(1.0f, 1.0f, 1.0f) == transform->GetLocalScale());
		}

		TEST_METHOD(CustomConstructor)
		{
			Vector3 position(1.0f, -2.0f, 3.0f);
			Quaternion rotation(1.0f, 0.0f, 0.0f, 1.0f);
			Vector3 scale(3.0f, 2.0f, 1.0f);
			auto transform = std::make_shared<TransformComponent>(position, rotation, scale);
			Assert::IsTrue(position == transform->GetLocalPosition());
			Assert::IsTrue(rotation.isApprox(transform->GetLocalRotation()));
			Assert::IsTrue(scale == transform->GetLocalScale());
		}

		TEST_METHOD(SetLocalTranslation)
		{
			auto transform = std::make_shared<TransformComponent>();

			Vector3 expectedTranslation(1.0f, -1.0f, 2.0f);
			transform->SetLocalPosition(expectedTranslation);
			Assert::IsTrue(expectedTranslation == transform->GetLocalPosition());
		}

		TEST_METHOD(SetLocalRotation)
		{
			auto transform = std::make_shared<TransformComponent>();
			Quaternion expectedRotation(AngleAxisf(std::acos(-1.0f), Vector3(0.0f, 1.0f, 0.0f)));
			transform->SetLocalRotation(expectedRotation);
			Assert::IsTrue(expectedRotation.coeffs() == (transform->GetLocalRotation().coeffs()));
		}

		TEST_METHOD(SetLocalScale)
		{
			auto transform = std::make_shared<TransformComponent>();
			Vector3 expectedScale(3.0f, 1.0f, 0.5f);
			transform->SetLocalScaling(expectedScale);
			Assert::IsTrue(expectedScale == transform->GetLocalScale());
		}

		TEST_METHOD(Translate)
		{
			auto transform = std::make_shared<TransformComponent>();

			transform->Move(Vector3({ 1.0f, 1.0f, 1.0f }), 2.0f);
			Assert::IsTrue(transform->GetLocalPosition() == Vector3({ 2.0f, 2.0f, 2.0f }));

			transform->Move(Vector3({ -1.0f, -1.0f, -1.0f }), 2.0f);
			Assert::IsTrue(transform->GetLocalPosition() == Vector3({ 0.0f, 0.0f, 0.0f }));

			transform->MoveLocalZ(1.0f);
			Assert::IsTrue(transform->GetLocalPosition() == Vector3({ 0.0f, 0.0f, 1.0f }));

			transform->MoveLocalY(-2.0f);
			Assert::IsTrue(transform->GetLocalPosition() == Vector3({ 0.0f, -2.0f, 1.0f }));

			transform->MoveLocalX(3.0f);
			Assert::IsTrue(transform->GetLocalPosition() == Vector3({ 3.0f, -2.0f, 1.0f }));

			transform->SetLocalRotation(Quaternionf(AngleAxisf(c_pi / 2.0f, Vector3::UnitZ())));
			transform->MoveLocalX(-4.0f);
			Assert::IsTrue(transform->GetLocalPosition().isApprox(Vector3({ 3.0f, -6.0f, 1.0f })));

			transform->MoveLocalY(5.0f);
			Assert::IsTrue(transform->GetLocalPosition().isApprox(Vector3({ -2.0f, -6.0f, 1.0f })));
		}

		TEST_METHOD(Rotate)
		{
			const auto pi = std::acos(-1.0f);

			auto transform = std::make_shared<TransformComponent>();
			transform->RotateLocalZ(pi / 2.0f);
			Assert::IsTrue(transform->GetLocalRotation().isApprox(Quaternionf(AngleAxisf(pi / 2.0f, Vector3::UnitZ()))));
		}

		TEST_METHOD(SettingParent)
		{
			{
				auto t1 = std::make_shared<TransformComponent>();
				auto worldTransformBeforeAddingParent = t1->GetWorldTransform();

				// World position stays = true:
				{
					auto parent = std::make_shared<TransformComponent>();
					parent->SetLocalPosition({ 0.0f, 1.0f, 2.0f });
					parent->SetLocalRotation(
						Quaternion(
							AngleAxisf(c_pi, Vector3f::UnitZ()) *
							AngleAxisf(c_pi, Vector3f::UnitY()) *
							AngleAxisf(c_pi, Vector3f::UnitZ())
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

		TEST_METHOD(WorldAndLocalSynch)
		{
			auto pi = std::acos(-1.0f);

			// Apply local transformations and test world position:
			{
				auto t1 = std::make_shared<TransformComponent>();
				t1->SetLocalPosition({ 1.0f, 1.0f, 0.0f });
				t1->SetLocalRotation(Quaternion(AngleAxisf(-c_pi / 2.0f, Vector3::UnitZ())));

				auto parent = std::make_shared<TransformComponent>();
				parent->SetLocalRotation(Quaternion(AngleAxisf(-c_pi / 2.0f, Vector3::UnitZ())));
				t1->SetParent(parent, false);

				Assert::IsTrue(t1->GetWorldPosition().isApprox(Vector3f(1.0f, -1.0f, 0.0f)));
			}

			// Apply local transformations and test world rotation:
			{
				auto t1 = std::make_shared<TransformComponent>();
				t1->SetLocalRotation(Quaternion(AngleAxisf(pi, Vector3f::UnitZ())));

				auto parent = std::make_shared<TransformComponent>();
				parent->SetLocalRotation(Quaternion(AngleAxisf(pi, Vector3f::UnitZ())));
				t1->SetParent(parent, false);

				Assert::IsTrue(t1->GetWorldRotation().isApprox(Quaternion(AngleAxisf(2.0f * pi, Vector3f::UnitZ()))));
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

			// Test that world transform is updated when parent transform changes:
			{
				auto parent = std::make_shared<TransformComponent>();
				parent->SetLocalPosition({ 1.0f, 2.0f, 3.0f });

				auto t1 = std::make_shared<TransformComponent>();
				t1->SetWorldPosition({ 4.0f, 5.0f, 6.0f });

				t1->SetParent(parent, false);
				Assert::IsTrue(t1->GetWorldPosition() == Vector3f(5.0f, 7.0f, 9.0f));

				Affine3f expectedWorldTransform;
				expectedWorldTransform.fromPositionOrientationScale(Vector3f(5.0f, 7.0f, 9.0f), Quaternionf::Identity(), Vector3f(1.0f, 1.0f, 1.0f));
				Assert::IsTrue(expectedWorldTransform.isApprox(t1->GetWorldTransform()));

				parent->SetLocalPosition({ 2.0f, 3.0f, 4.0f });
				Assert::IsTrue(t1->GetWorldPosition() == Vector3f(6.0f, 8.0f, 10.0f));

				expectedWorldTransform.fromPositionOrientationScale(Vector3f(6.0f, 8.0f, 10.0f), Quaternionf::Identity(), Vector3f(1.0f, 1.0f, 1.0f));
				Assert::IsTrue(expectedWorldTransform.isApprox(t1->GetWorldTransform()));
			}
		}
	};
}
