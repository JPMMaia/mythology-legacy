#include "stdafx.h"
#include "CppUnitTest.h"
#include "GameEngine/GameManager.h"
#include "Common/Timer.h"
#include "GameEngine/GameObject/GameObject.h"
#include "GameEngine/Component/Lights/PointLightComponent.h"
#include "GameEngine/Component/Cameras/CameraComponent.h"

using namespace Eigen;
using namespace GameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MythologyTestProject
{
	TEST_CLASS(GameManagerTests)
	{
	private:
		template<class T>
		using Allocator = StandardAllocator<T>;

	public:
		TEST_METHOD(GameManagerTest1)
		{
			Common::Timer timer(std::chrono::milliseconds(12));
			
			GameManager gameManager;

			GameObject gameObject1;

			/*auto boxComponent = std::allocate_shared<MeshComponent<BoxGeometry>>(Allocator<MeshComponent<BoxGeometry>>(), BoxGeometry(1.0f, 1.0f, 1.0f, 0));
			gameObject1.AddComponent("Mesh", *boxComponent);

			auto pointLightComponent = std::allocate_shared<PointLightComponent>(Allocator<PointLightComponent>(), Vector3f(0.8f, 0.8f, 0.8f), 2.0f, 5.0f);
			pointLightComponent->SetLocalPosition({ 0.0f, 2.0f, -1.0f });
			gameObject1.AddComponent("Light", *pointLightComponent);*/

			auto cameraComponent = std::allocate_shared<CameraComponent>(Allocator<CameraComponent>());
			cameraComponent->GetTransform().SetLocalPosition({ 0.0f, 2.0f, -1.0f });
			cameraComponent->GetTransform().SetLocalRotation(Quaternionf::FromTwoVectors( Vector3f::UnitZ(), Vector3f(0.0f, -2.0f, 1.0f) ));
			gameObject1.AddComponent("Camera", *cameraComponent);

			gameManager.FixedUpdate(timer);

			Assert::IsTrue(cameraComponent->GetViewMatrix().isApprox( cameraComponent->GetTransform().GetWorldRotation().toRotationMatrix() * Translation3f(Vector3f( 0.0f, -2.0f, 1.0f )) ));
		}
	};
}
