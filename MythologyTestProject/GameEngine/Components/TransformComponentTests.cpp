#include "stdafx.h"
#include "CppUnitTest.h"
#include "GameEngine/Component/TransformComponent.h"
#include "Libraries/tue/transform.hpp"
#include "Common/Timer.h"

#include <cmath>

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
				tue::fvec3 expectedTranslation(1.0f, -1.0f, 2.0f);
				t1->SetTranslation(expectedTranslation);
				Assert::IsTrue(expectedTranslation == t1->Translation());
			}

			// Test rotation:
			auto pi = std::acos(-1.0f);
			{
				auto expectedRotation = tue::transform::rotation_quat(pi, pi / 2.0f, 0.1f * pi);
				t1->SetRotation(expectedRotation);
				Assert::IsTrue(expectedRotation == t1->Rotation());
			}

			// Test scale:
			{
				tue::fvec3 expectedScale(3.0f, 1.0f, 0.5f);
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
					t1->SetParent(parent, false);

					t1->FixedUpdate(timer);

					auto expectedWorldTransform = parent->WorldTransform() * worldTransformBeforeAddingParent;
					Assert::IsTrue(t1->WorldTransform() == expectedWorldTransform);
				}

				// World position stays = true:
				{
					auto parent = std::make_shared<TransformComponent>();
					parent->SetTranslation({ 0.0f, 1.0f, 2.0f });
					t1->SetParent(parent, true);

					t1->FixedUpdate(timer);

					Assert::IsTrue(t1->WorldTransform() == worldTransformBeforeAddingParent);
				}
			}
		}
	};
}