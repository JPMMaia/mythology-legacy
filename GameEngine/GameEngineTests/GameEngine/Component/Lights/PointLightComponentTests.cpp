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
	TEST_CLASS(PointLightComponentTests)
	{
	public:
		using Vector3 = PointLightComponent::Vector3Type;

	public:
		TEST_METHOD(DefaultConstructor)
		{
			PointLightComponent light;
			Assert::IsTrue(light.GetStrength() == Vector3::Zero());
			Assert::IsTrue(light.GetFalloffStart() == 0.0f);
			Assert::IsTrue(light.GetFalloffEnd() == 0.0f);
		}

		TEST_METHOD(CustomConstructor)
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

		TEST_METHOD(TransformIntegration)
		{
			Vector3 position(1.0f, 2.0f, 3.0f);

			PointLightComponent light;
			light.GetTransform().SetWorldPosition(position);
			Assert::IsTrue(light.GetWorldPosition() == position);
		}
	};
}
