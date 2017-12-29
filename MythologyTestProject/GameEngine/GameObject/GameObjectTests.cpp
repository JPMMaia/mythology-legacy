#include "pch.h"
#include "CppUnitTest.h"
#include "GameEngine/GameObject/GameObject.h"
#include "GameEngine/Component/Lights/PointLightComponent.h"

using namespace Eigen;
using namespace GameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MythologyTestProject
{
	class TestComponent : public BaseComponent
	{		
	};

	TEST_CLASS(GameObjectTests)
	{
	public:
		TEST_METHOD(RootComponent)
		{
			{
				auto root = std::make_shared<TransformComponent>();

				GameObject gameObject;
				gameObject.AddRootComponent("Root", root);
				Assert::IsTrue(root == gameObject.GetSharedTransform());

				auto removeRoot = [&gameObject]()
				{
					gameObject.RemoveComponent("Root");
				};
				Assert::ExpectException<std::invalid_argument>(removeRoot);
			}

			{
				auto root = std::make_shared<TestComponent>();

				GameObject gameObject;
				gameObject.AddRootComponent("Root", root);
				Assert::IsTrue(root->GetSharedTransform() == gameObject.GetSharedTransform());

				auto removeRoot = [&gameObject]()
				{
					gameObject.RemoveComponent("Root");
				};
				Assert::ExpectException<std::invalid_argument>(removeRoot);
			}
		}

		TEST_METHOD(Component)
		{
			GameObject gameObject;

			auto root = std::make_shared<TestComponent>();
			gameObject.AddRootComponent("Root", root);

			auto component0 = std::make_shared<TestComponent>();
			gameObject.AddComponent("0", component0);
			Assert::IsTrue(root->GetSharedTransform() == component0->GetSharedTransform()->GetParent().lock());

			auto component1 = std::make_shared<TestComponent>();
			gameObject.AddComponent("1", component1);
			Assert::IsTrue(root->GetSharedTransform() == component1->GetSharedTransform()->GetParent().lock());

			gameObject.RemoveComponent("0");
			Assert::IsTrue(component0->GetSharedTransform()->GetParent().lock() == nullptr);
		}
	};
}