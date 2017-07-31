#include "stdafx.h"
#include "CppUnitTest.h"
#include "GameEngine/Component/Lights/DirectionalLightComponent.h"

using namespace Eigen;
using namespace GameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MythologyTestProject
{
	TEST_CLASS(LightComponentTests)
	{
	public:
		TEST_METHOD(DirectionalLightComponentTests1)
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
	};
}