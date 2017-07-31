#include "stdafx.h"
#include "CppUnitTest.h"
#include "GameEngine/Component/Lights/DirectionalLightComponent.h"
#include "GameEngine/Component/Lights/PointLightComponent.h"
#include "GameEngine/Component/Lights/SpotLightComponent.h"

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
				Assert::IsTrue(light.Strength() == Vector3::Zero());
				Assert::IsTrue(light.Direction() == Vector3::Zero());
			}

			// Test custom constructor and accessors:
			{
				Vector3 strength(1.0f, 2.0f, 3.0f);
				Vector3 direction(4.0f, 5.0f, 6.0f);
				DirectionalLightComponent light(strength, direction);
				Assert::IsTrue(light.Strength() == strength);
				Assert::IsTrue(light.Direction() == direction);

				Vector3 newStrength(-4.0f, -5.0f, -6.0f);
				light.Strength() = newStrength;
				Assert::IsTrue(light.Strength() == newStrength);

				Vector3 newDirection(-1.0f, -2.0f, -3.0f);
				light.Direction() = newDirection;
				Assert::IsTrue(light.Direction() == newDirection);
			}
		}

		TEST_METHOD(PointLightComponentTests)
		{
			using Vector3 = PointLightComponent::Vector3Type;

			// Test default constructor:
			{
				PointLightComponent light;
				Assert::IsTrue(light.Strength() == Vector3::Zero());
				Assert::IsTrue(light.Position() == Vector3::Zero());
				Assert::IsTrue(light.FalloffStart() == 0.0f);
				Assert::IsTrue(light.FalloffEnd() == 0.0f);
			}

			// Test custom constructor and accessors:
			{
				Vector3 strength(1.0f, 2.0f, 3.0f);
				Vector3 position(4.0f, 5.0f, 6.0f);
				auto falloffStart = 3.0f;
				auto falloffEnd = 5.0f;
				PointLightComponent light(strength, position, falloffStart, falloffEnd);
				Assert::IsTrue(light.Strength() == strength);
				Assert::IsTrue(light.Position() == position);
				Assert::IsTrue(light.FalloffStart() == falloffStart);
				Assert::IsTrue(light.FalloffEnd() == falloffEnd);

				Vector3 newStrength(2.0f, 3.0f, 4.0f);
				light.Strength() = newStrength;
				Assert::IsTrue(light.Strength() == newStrength);

				Vector3 newPosition(-4.0f, 1.0f, -3.0f);
				light.Position() = newPosition;
				Assert::IsTrue(light.Position() == newPosition);

				auto newFalloffStart = 7.0f;
				light.FalloffStart() = newFalloffStart;
				Assert::IsTrue(light.FalloffStart() == newFalloffStart);

				auto newFalloffEnd = 15.0f;
				light.FalloffEnd() = newFalloffEnd;
				Assert::IsTrue(light.FalloffEnd() == newFalloffEnd);
			}
		}

		TEST_METHOD(SpotLightComponentTests)
		{
			using Vector3 = SpotLightComponent::Vector3Type;

			// Test default constructor:
			{
				SpotLightComponent light;
				Assert::IsTrue(light.Strength() == Vector3::Zero());
				Assert::IsTrue(light.Position() == Vector3::Zero());
				Assert::IsTrue(light.Direction() == Vector3::Zero());
				Assert::IsTrue(light.FalloffStart() == 0.0f);
				Assert::IsTrue(light.FalloffEnd() == 0.0f);
				Assert::IsTrue(light.SpotPower() == 0.0f);
			}

			// Test custom constructor and accessors:
			{
				Vector3 strength(1.0f, 2.0f, 3.0f);
				Vector3 position(4.0f, 5.0f, 6.0f);
				Vector3 direction(2.0f, 1.0f, -6.0f);
				auto falloffStart = 3.0f;
				auto falloffEnd = 5.0f;
				auto spotPower = 32.0f;
				SpotLightComponent light(strength, position, direction, falloffStart, falloffEnd, spotPower);
				Assert::IsTrue(light.Strength() == strength);
				Assert::IsTrue(light.Position() == position);
				Assert::IsTrue(light.Direction() == direction);
				Assert::IsTrue(light.FalloffStart() == falloffStart);
				Assert::IsTrue(light.FalloffEnd() == falloffEnd);
				Assert::IsTrue(light.SpotPower() == spotPower);

				Vector3 newStrength(2.0f, 3.0f, 4.0f);
				light.Strength() = newStrength;
				Assert::IsTrue(light.Strength() == newStrength);

				Vector3 newPosition(-4.0f, 1.0f, -3.0f);
				light.Position() = newPosition;
				Assert::IsTrue(light.Position() == newPosition);

				Vector3 newDirection(-1.0f, -2.0f, -3.0f);
				light.Direction() = newDirection;
				Assert::IsTrue(light.Direction() == newDirection);

				auto newFalloffStart = 7.0f;
				light.FalloffStart() = newFalloffStart;
				Assert::IsTrue(light.FalloffStart() == newFalloffStart);

				auto newFalloffEnd = 15.0f;
				light.FalloffEnd() = newFalloffEnd;
				Assert::IsTrue(light.FalloffEnd() == newFalloffEnd);

				auto newSpotPower = 64.0f;
				light.SpotPower() = newSpotPower;
				Assert::IsTrue(light.SpotPower() == newSpotPower);
			}
		}
	};
}