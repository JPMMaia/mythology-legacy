#include "stdafx.h"
#include "CppUnitTest.h"
#include "GameEngine/GameObject/GameObject.h"
#include "GameEngine/Component/Lights/PointLightComponent.h"
#include "Common/Timer.h"

using namespace GameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MythologyTestProject
{
	TEST_CLASS(GameObjectTests)
	{
	public:
		TEST_METHOD(GameObjectTest1)
		{
			using Vector3 = Eigen::Vector3f;
			using Quaternion = Eigen::Quaternion<float>;

			GameObject gameObject;

			// Test default construction:
			{
				auto& transform = gameObject.Transform();
				Assert::IsTrue(transform.LocalPosition() == Vector3::Zero());
				Assert::IsTrue(transform.LocalRotation().isApprox(Quaternion::Identity()));
				Assert::IsTrue(transform.LocalScaling() == Vector3(1.0f, 1.0f, 1.0f));
			}

			// Test components:
			PointLightComponent pointLightComponent;
			{
				gameObject.AddComponent("PointLight", pointLightComponent);
				auto& component = gameObject.GetComponent("PointLight");
				Assert::IsTrue(&component == &pointLightComponent);

				gameObject.RemoveComponent("PointLight");
				Assert::IsFalse(gameObject.HasComponent("PointLight"));
			}

			// Test transform logic:
			{
				Common::Timer timer(std::chrono::milliseconds(12));

				gameObject.AddComponent("PointLight", pointLightComponent);

				auto& transform = gameObject.Transform();
				transform.SetLocalPosition({1.0f, -2.0f, 5.0f});
				transform.FixedUpdate(timer);
				// TODO

			}
		}
	};
}