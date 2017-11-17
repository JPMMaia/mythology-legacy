#include "pch.h"
#include "CppUnitTest.h"
#include "GameEngine/GameObject/GameObject.h"
#include "GameEngine/Component/Lights/PointLightComponent.h"

using namespace Eigen;
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
				auto& transform = gameObject.GetTransform();
				Assert::IsTrue(transform.GetLocalPosition() == Vector3::Zero());
				Assert::IsTrue(transform.GetLocalRotation().isApprox(Quaternion::Identity()));
				Assert::IsTrue(transform.GetLocalScale() == Vector3(1.0f, 1.0f, 1.0f));
			}

			// Test components:
			auto pointLightComponent = PointLightComponent::CreateSharedPointer();
			{
				gameObject.AddComponent("PointLight", pointLightComponent);
				const auto& component = gameObject.GetComponent("PointLight");
				Assert::IsTrue(component.get() == pointLightComponent.get());

				gameObject.RemoveComponent("PointLight");
				Assert::IsFalse(gameObject.HasComponent("PointLight"));

				// Remove non-existent component:
				gameObject.RemoveComponent("OtherComponent");
			}

			// Test transform logic:
			{
				gameObject.AddComponent("PointLight", pointLightComponent);
				pointLightComponent->SetLocalPosition({ 1.0f, 1.0f, 1.0f });

				auto& transform = gameObject.GetTransform();
				transform.SetLocalPosition({1.0f, -2.0f, 5.0f});
				
				Vector3f worldPosition(2.0f, -1.0f, 6.0f);
				Assert::IsTrue(pointLightComponent->GetWorldPosition() == worldPosition);

				gameObject.RemoveComponent("PointLight", true);
				Assert::IsTrue(pointLightComponent->GetTransform().GetParent().expired());
				Assert::IsTrue(pointLightComponent->GetWorldPosition() == worldPosition);
			}
		}
	};
}