#include "stdafx.h"
#include "CppUnitTest.h"
#include "Common/MemoryPool/Allocator.h"
#include "GameEngine/GameObject/GameObject.h"

#include <vector>
#include "GameEngine/Component/Cameras/CameraComponent.h"

using namespace GameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MythologyTestProject
{
	TEST_CLASS(StandardAllocatorTests)
	{
	public:
		TEST_METHOD(StandardAllocatorTest1)
		{
			std::allocator<GameObject> allocator;

			static constexpr std::size_t gameObjectsCount = 1;
			std::vector<std::shared_ptr<GameObject>> gameObjects(gameObjectsCount);
			for (std::size_t i = 0; i < gameObjectsCount; ++i)
				gameObjects.emplace_back(std::allocate_shared<GameObject>(allocator));

			// Erase at middle:
			//gameObjects.erase(gameObjects.begin() + 7);

			// Check if pointer is still valid:
			auto camera = std::make_shared<CameraComponent>();
			{
				const auto& gameObject = gameObjects[0];
				gameObject->AddComponent("Camera", *camera);
				Assert::IsTrue(gameObject->HasComponent("Camera"));
			}
			
			/*// Check if removed node is initialized again:
			Assert::AreEqual(std::size_t(10), allocator.size());
			auto anotherGameObject = std::allocate_shared<GameObject>(allocator);
			Assert::AreEqual(std::size_t(10), allocator.size());
			Assert::IsFalse(anotherGameObject->HasComponent("Camera"));

			// Erase at end:
			gameObjects.erase(gameObjects.begin() + 7, gameObjects.end());

			// Check if size decreased:
			Assert::AreEqual(std::size_t(8), allocator.size());*/
		}
	};
}