#include "pch.h"
#include "CppUnitTest.h"
#include "GameEngine/Component/Lights/DirectionalLightComponent.h"
#include "GameEngine/Component/Lights/PointLightComponent.h"
#include "GameEngine/Component/Lights/SpotLightComponent.h"
#include "GameEngine/GameObject/GameObject.h"

using namespace Eigen;
using namespace GameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MythologyTestProject
{
	TEST_CLASS(DirectionalLightComponentTests)
	{
	public:
		using Vector3 = DirectionalLightComponent::Vector3Type;

	public:
		TEST_METHOD(DefaultConstructor)
		{
			DirectionalLightComponent light;
			Assert::IsTrue(light.GetStrength() == Vector3::Zero());
			Assert::IsTrue(light.GetLocalDirection() == Vector3f(0.0f, -1.0f, 0.0f));
		}

		TEST_METHOD(CustomConstructor)
		{
			Vector3 strength(1.0f, 2.0f, 3.0f);
			Vector3 direction(4.0f, 5.0f, 6.0f);
			DirectionalLightComponent light(strength);
			light.SetLocalDirection(direction);
			Assert::IsTrue(light.GetStrength() == strength);
			Assert::IsTrue(light.GetLocalDirection().isApprox(direction.normalized()));

			Vector3 newStrength(-4.0f, -5.0f, -6.0f);
			light.SetStrength(newStrength);
			Assert::IsTrue(light.GetStrength() == newStrength);

			Vector3 newDirection(-1.0f, -2.0f, -3.0f);
			light.SetLocalDirection(newDirection);
			Assert::IsTrue(light.GetLocalDirection().isApprox(newDirection.normalized()));
		}

		TEST_METHOD(TransformIntegration)
		{
			DirectionalLightComponent light;

			{
				Vector3f direction({ 0.0f, -2.0f, 0.0f });
				light.SetLocalDirection(direction);
				Assert::IsTrue(light.GetLocalDirection() == direction.normalized());
			}

			{
				auto pi = std::acos(-1.0f);
				light.GetTransform().SetLocalRotation(Quaternionf(AngleAxisf(-pi / 2.0f, Vector3f::UnitX())));
				Assert::IsTrue(light.GetLocalDirection().isApprox(Vector3f(0.0f, 0.0f, 1.0f)));

				light.SetLocalDirection({ 0.0f, 0.0f, -1.0f });
				Assert::IsTrue(light.GetTransform().GetWorldRotation().isApprox(Quaternionf(AngleAxisf(pi / 2.0f, Vector3f::UnitX()))));
			}
		}
	};
}
