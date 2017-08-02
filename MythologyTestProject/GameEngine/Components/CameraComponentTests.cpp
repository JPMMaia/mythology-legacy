#include "stdafx.h"
#include "CppUnitTest.h"
#include "GameEngine/Component/Cameras/CameraComponent.h"

using namespace Eigen;
using namespace GameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MythologyTestProject
{
	TEST_CLASS(CameraComponentTests)
	{
	public:
		TEST_METHOD(CameraComponentTest1)
		{
			CameraComponent camera;

			// Test local space:
			{
				auto parent = std::make_shared<TransformComponent>();
				camera.SetParent(parent, false);


			}
		}
	};
}
