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
	private:
		template<class T>
		using Allocator = StandardAllocator<T>;

	public:
		TEST_METHOD(StandardAllocatorTest1)
		{
			// Test destruction:
			{
				auto test = CameraComponent::CreateSharedPointer();
				Assert::AreEqual(std::size_t(1), CameraComponent::GetStorage().size());
			}
			Assert::AreEqual(std::size_t(0), CameraComponent::GetStorage().size());

			static constexpr std::size_t componentCount = 10;
			std::vector<std::shared_ptr<CameraComponent>> components;
			components.reserve(componentCount);
			for (std::size_t i = 0; i < componentCount; ++i)
				components.emplace_back(CameraComponent::CreateSharedPointer());	
			Assert::AreEqual(componentCount, CameraComponent::GetStorage().size());
			
			// Set fov angle for a component:
			components[8]->SetFovAngleY(90.0f);

			// Erase at middle:
			components.erase(components.begin() + 7);

			// Check if pointer is still valid:
			Assert::IsTrue(90.0f == components[7]->GetFovAngleY());

			// Check if removed node is initialized again:
			Assert::AreEqual(std::size_t(10), CameraComponent::GetStorage().size());
			auto anotherComponentPointer = new CameraComponent;
			auto anotherComponent = std::shared_ptr<CameraComponent>(anotherComponentPointer);
			Assert::AreEqual(std::size_t(10), CameraComponent::GetStorage().size());
			anotherComponent->SetFarZ(70.0f);
			Assert::IsTrue(anotherComponent->GetFarZ() == 70.0f);

			// Erase at end:
			components.erase(components.begin() + 7, components.end());

			// Check if size decreased:
			Assert::AreEqual(std::size_t(7), CameraComponent::GetStorage().size());
		}
	};
}