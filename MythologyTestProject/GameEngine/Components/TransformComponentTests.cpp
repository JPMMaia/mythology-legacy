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
				t1->SetTranslation(expectedTranslation);
				Assert::IsTrue(expectedTranslation == t1->Translation());
			}

			// Test rotation:
			auto pi = std::acos(-1.0f);
			{
				Quaternionf expectedRotation(AngleAxisf(pi, Vector3f(0.0f, 1.0f, 0.0f)));
				t1->SetRotation(expectedRotation);
				Assert::IsTrue(expectedRotation.coeffs() == (t1->Rotation().coeffs()));
			}

			// Test scale:
			{
				Vector3f expectedScale(3.0f, 1.0f, 0.5f);
				t1->SetScale(expectedScale);
				Assert::IsTrue(expectedScale == t1->Scale());
			}

			// Test setting parent:
			{
				Common::Timer timer(std::chrono::milliseconds(12));
				t1->FixedUpdate(timer);
				auto worldTransformBeforeAddingParent = t1->WorldTransform();

				// World position stays = false:
				{
					auto parent = std::make_shared<TransformComponent>();
					parent->SetTranslation({ 0.0f, 1.0f, 2.0f });
					parent->SetRotation(Quaternion<float>(1.0f, 0.1f, 0.4f, 0.3f).normalized());
					parent->SetScale({ 0.1f, 0.4f, 0.6f });
					t1->SetParent(parent, false);

					t1->FixedUpdate(timer);

					auto expectedWorldTransform = parent->WorldTransform() * worldTransformBeforeAddingParent;
					Assert::IsTrue(t1->WorldTransform().matrix() == expectedWorldTransform.matrix());
				}

				// World position stays = true:
				{
					auto parent = std::make_shared<TransformComponent>();
					parent->SetTranslation({ 0.0f, 1.0f, 2.0f });
					parent->SetRotation(
						Quaternion<float>(
							AngleAxisf(pi, Vector3f::UnitZ()) *
							AngleAxisf(pi, Vector3f::UnitY()) *
							AngleAxisf(pi, Vector3f::UnitZ())
							)
					);
					parent->SetScale({ 0.1f, 0.4f, 0.6f });
					t1->SetParent(parent, true);

					t1->FixedUpdate(timer);

					Assert::IsTrue(t1->WorldTransform().isApprox(worldTransformBeforeAddingParent));
				}
			}
		}
	};
}