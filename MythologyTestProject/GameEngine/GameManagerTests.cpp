#include "stdafx.h"
#include "CppUnitTest.h"
#include "GameEngine/GameManager.h"
#include "Common/Timer.h"
#include "GameEngine/GameObject/GameObject.h"
#include "GameEngine/Component/Lights/PointLightComponent.h"
#include "GameEngine/Component/Cameras/CameraComponent.h"
#include "GameEngine/Component/Meshes/MeshComponent.h"
#include "GameEngine/Geometry/Primitives/BoxGeometry.h"

using namespace Eigen;
using namespace GameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MythologyTestProject
{
	TEST_CLASS(GameManagerTests)
	{
	private:
		template<class T>
		using Allocator = std::allocator<T>;

	public:
		TEST_METHOD(GameManagerTest1)
		{
			Common::Timer timer(std::chrono::milliseconds(12));
			
			GameManager gameManager;

			GameObject gameObject1;

			auto boxComponentPointer = new MeshComponent<BoxGeometry>(BoxGeometry(1.0f, 1.0f, 1.0f, 0));
			std::shared_ptr<MeshComponent<BoxGeometry>> boxComponent(boxComponentPointer);
			gameObject1.AddComponent("Mesh", boxComponent);

			auto pointLightComponentPointer = new PointLightComponent(Vector3f(0.8f, 0.8f, 0.8f), 2.0f, 5.0f);
			std::shared_ptr<PointLightComponent> pointLightComponent(pointLightComponentPointer);
			pointLightComponent->SetLocalPosition({ 0.0f, 2.0f, -1.0f });
			gameObject1.AddComponent("Light", pointLightComponent);

			auto cameraComponentPointer = new CameraComponent();
			std::shared_ptr<CameraComponent> cameraComponent(cameraComponentPointer);
			cameraComponent->GetTransform().SetLocalPosition({ 0.0f, 2.0f, -1.0f });
			cameraComponent->GetTransform().SetLocalRotation(Quaternionf::FromTwoVectors( Vector3f::UnitZ(), Vector3f(0.0f, -2.0f, 1.0f) ));
			gameObject1.AddComponent("Camera", cameraComponent);

			gameManager.FixedUpdate(timer);

			Assert::IsTrue(cameraComponent->GetViewMatrix().isApprox((Translation3f(Vector3f(0.0f, 2.0f, -1.0f)) * cameraComponent->GetTransform().GetWorldRotation().toRotationMatrix()).inverse()));
		}
	};
}
