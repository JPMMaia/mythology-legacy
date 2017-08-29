#include "pch.h"
#include "CppUnitTest.h"
#include "GameEngine/Component/Physics/PhysicsComponent.h"

using namespace GameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MythologyTestProject
{
	TEST_CLASS(PhysicsComponentTests)
	{
	public:
		TEST_METHOD(PhysicsComponentTest1)
		{
			PhysicsComponent physicsComponent;
		}
	};
}
