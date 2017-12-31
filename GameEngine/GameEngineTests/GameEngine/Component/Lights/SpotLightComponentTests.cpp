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
	TEST_CLASS(SpotLightComponentTests)
	{
	public:
		using Vector3 = SpotLightComponent::Vector3Type;

	public:
		TEST_METHOD(DefaultConstructor)
		{
			SpotLightComponent light;
			Assert::IsTrue(light.GetStrength() == Vector3::Zero());
			Assert::IsTrue(light.GetFalloffStart() == 0.0f);
			Assert::IsTrue(light.GetFalloffEnd() == 0.0f);
			Assert::IsTrue(light.GetSpotPower() == 0.0f);
			Assert::IsTrue(light.GetLocalPosition() == Vector3::Zero());
			Assert::IsTrue(light.GetLocalDirection() == Vector3(0.0f, -1.0f, 0.0f));
		}

		TEST_METHOD(CustomConstructor)
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
	};
}
