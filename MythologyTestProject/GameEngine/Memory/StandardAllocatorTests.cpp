#include "stdafx.h"
#include "CppUnitTest.h"

#include <vector>
#include "GameEngine/Component/Cameras/CameraComponent.h"
#include "GameEngine/Memory/StandardAllocator.h"
#include <windows.h>

using namespace GameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MythologyTestProject
{
	TEST_CLASS(StandardAllocatorTests)
	{
	public:
		TEST_METHOD(StandardAllocatorTest1)
		{
			static constexpr std::size_t componentCount = 10;
			std::vector<std::shared_ptr<CameraComponent>> components;
			components.reserve(componentCount);
			for (std::size_t i = 0; i < componentCount; ++i)
			{
				auto componentPointer = new CameraComponent;
				components.emplace_back(std::shared_ptr<CameraComponent>(componentPointer));
			}	
			Assert::AreEqual(componentCount, StandardAllocator<CameraComponent>::size());
			
			// Set fov angle for a component:
			components[8]->SetFovAngleY(90.0f);

			// Erase at middle:
			components.erase(components.begin() + 7);

			// Check if pointer is still valid:
			Assert::IsTrue(90.0f == components[7]->GetFovAngleY());

			// Check if removed node is initialized again:
			Assert::AreEqual(std::size_t(10), StandardAllocator<CameraComponent>::size());
			auto anotherComponentPointer = new CameraComponent;
			auto anotherComponent = std::shared_ptr<CameraComponent>(anotherComponentPointer);
			Assert::AreEqual(std::size_t(10), StandardAllocator<CameraComponent>::size());
			anotherComponent->SetFarZ(70.0f);
			Assert::IsFalse(anotherComponent->GetFarZ() == 70.0f);

			// Erase at end:
			components.erase(components.begin() + 7, components.end());

			// Check if size decreased:
			Assert::AreEqual(std::size_t(8), StandardAllocator<CameraComponent>::size());
		}
	};
}