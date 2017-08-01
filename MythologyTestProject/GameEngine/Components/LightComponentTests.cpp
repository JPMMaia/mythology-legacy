#include "stdafx.h"
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
	TEST_CLASS(LightComponentTests)
	{
	public:
		TEST_METHOD(DirectionalLightComponentTests)
		{
			using Vector3 = DirectionalLightComponent::Vector3Type;

			// Test default constructor:
			{
				DirectionalLightComponent light;
				Assert::IsTrue(light.GetStrength() == Vector3::Zero());
				Assert::IsTrue(light.GetLocalDirection() == Vector3f(0.0f, -1.0f, 0.0f));
			}

			// Test custom constructor and accessors:
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

			// Test integration with transform:
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

					light.SetLocalDirection({ 0.0f, 0.0f, -1.0f});
					Assert::IsTrue(light.GetTransform().GetWorldRotation().isApprox(Quaternionf(AngleAxisf(pi / 2.0f, Vector3f::UnitX()))));
				}
			}
		}

		TEST_METHOD(PointLightComponentTests)
		{
			using Vector3 = PointLightComponent::Vector3Type;

			// Test default constructor:
			{
				PointLightComponent light;
				Assert::IsTrue(light.GetStrength() == Vector3::Zero());
				Assert::IsTrue(light.GetFalloffStart() == 0.0f);
				Assert::IsTrue(light.GetFalloffEnd() == 0.0f);
			}

			// Test custom constructor and accessors:
			{
				Vector3 strength(1.0f, 2.0f, 3.0f);
				auto falloffStart = 3.0f;
				auto falloffEnd = 5.0f;
				PointLightComponent light(strength, falloffStart, falloffEnd);
				Assert::IsTrue(light.GetStrength() == strength);
				Assert::IsTrue(light.GetFalloffStart() == falloffStart);
				Assert::IsTrue(light.GetFalloffEnd() == falloffEnd);

				Vector3 newStrength(2.0f, 3.0f, 4.0f);
				light.SetStrength(newStrength);
				Assert::IsTrue(light.GetStrength() == newStrength);

				auto newFalloffStart = 7.0f;
				light.SetFalloffStart(newFalloffStart);
				Assert::IsTrue(light.GetFalloffStart() == newFalloffStart);

				auto newFalloffEnd = 15.0f;
				light.SetFalloffEnd(newFalloffEnd);
				Assert::IsTrue(light.GetFalloffEnd() == newFalloffEnd);
			}

			// Test integration with transform:
			{
				Vector3 position(1.0f, 2.0f, 3.0f);

				PointLightComponent light;
				light.GetTransform().SetWorldPosition(position);
				Assert::IsTrue(light.GetWorldPosition() == position);
			}
		}

		TEST_METHOD(SpotLightComponentTests)
		{
			using Vector3 = SpotLightComponent::Vector3Type;

			// Test default constructor:
			{
				SpotLightComponent light;
				Assert::IsTrue(light.GetStrength() == Vector3::Zero());
				Assert::IsTrue(light.GetFalloffStart() == 0.0f);
				Assert::IsTrue(light.GetFalloffEnd() == 0.0f);
				Assert::IsTrue(light.GetSpotPower() == 0.0f);
				Assert::IsTrue(light.GetLocalPosition() == Vector3::Zero());
				Assert::IsTrue(light.GetLocalDirection() == Vector3(0.0f, -1.0f, 0.0f));
			}

			// Test custom constructor and accessors:
			{
				Vector3 strength(1.0f, 2.0f, 3.0f);
				auto falloffStart = 3.0f;
				auto falloffEnd = 5.0f;
				auto spotPower = 32.0f;
				SpotLightComponent light(strength, falloffStart, falloffEnd, spotPower);
				Assert::IsTrue(light.GetStrength() == strength);
				Assert::IsTrue(light.GetFalloffStart() == falloffStart);
				Assert::IsTrue(light.GetFalloffEnd() == falloffEnd);
				Assert::IsTrue(light.GetSpotPower() == spotPower);

				Vector3 newStrength(2.0f, 3.0f, 4.0f);
				light.SetStrength(newStrength);
				Assert::IsTrue(light.GetStrength() == newStrength);

				auto newFalloffStart = 7.0f;
				light.SetFalloffStart(newFalloffStart);
				Assert::IsTrue(light.GetFalloffStart() == newFalloffStart);

				auto newFalloffEnd = 15.0f;
				light.SetFalloffEnd(newFalloffEnd);
				Assert::IsTrue(light.GetFalloffEnd() == newFalloffEnd);

				auto newSpotPower = 64.0f;
				light.SetSpotPower(newSpotPower);
				Assert::IsTrue(light.GetSpotPower() == newSpotPower);

				Vector3 newPosition(-4.0f, 1.0f, -3.0f);
				light.SetLocalPosition(newPosition);
				Assert::IsTrue(light.GetLocalPosition() == newPosition);

				Vector3 newDirection(-1.0f, -2.0f, -3.0f);
				light.SetLocalDirection(newDirection);
				Assert::IsTrue(light.GetLocalDirection().isApprox(newDirection.normalized()));
			}
		}
	};
}
