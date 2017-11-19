#include "pch.h"
#include "CppUnitTest.h"

#include "GameEngine/Memory/UseStorage.h"

#include <vector>

using namespace GameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MythologyTestProject
{
	class StandardAllocatorTestComponent : public UseStorage<StandardAllocatorTestComponent>
	{
	public:
		int Value;
	};

	TEST_CLASS(StandardAllocatorTests)
	{
	private:
		template<class T>
		using Allocator = StandardAllocator<T>;

	public:
		TEST_METHOD_CLEANUP(CleanUp)
		{
			StandardAllocatorTestComponent::Clear();
		}

		TEST_METHOD(Destructor)
		{
			{
				auto component = StandardAllocatorTestComponent::CreateSharedPointer();
				Assert::AreEqual(std::size_t(1), StandardAllocatorTestComponent::GetStorage().size());
			}
			Assert::AreEqual(std::size_t(0), StandardAllocatorTestComponent::GetStorage().size());
		}

		TEST_METHOD(Erase)
		{
			static constexpr std::size_t componentCount = 10;
			std::vector<std::shared_ptr<StandardAllocatorTestComponent>> components;
			components.reserve(componentCount);
			for (std::size_t i = 0; i < componentCount; ++i)
				components.emplace_back(StandardAllocatorTestComponent::CreateSharedPointer());
			Assert::AreEqual(componentCount, StandardAllocatorTestComponent::GetStorage().size());
			
			// Set a value for a component
			components[8]->Value = 90;

			// Erase at middle:
			components.erase(components.begin() + 7);

			// Size of storage does not decrease:
			Assert::AreEqual(std::size_t(10), StandardAllocatorTestComponent::GetStorage().size());

			// Check if pointer is still valid:
			Assert::AreEqual(90, components[7]->Value);

			// Erase at end:
			components.erase(components.begin() + 7, components.end());

			// Check if size decreased:
			Assert::AreEqual(std::size_t(7), StandardAllocatorTestComponent::GetStorage().size());
		}

		TEST_METHOD(Reuse)
		{
			static constexpr std::size_t componentCount = 10;
			std::vector<std::shared_ptr<StandardAllocatorTestComponent>> components;
			components.reserve(componentCount);
			for (std::size_t i = 0; i < componentCount; ++i)
				components.emplace_back(StandardAllocatorTestComponent::CreateSharedPointer());
			Assert::AreEqual(componentCount, StandardAllocatorTestComponent::GetStorage().size());

			// Erase at middle:
			components.erase(components.begin() + 7);

			// Check if removed node is initialized again:
			auto anotherComponent = StandardAllocatorTestComponent::CreateSharedPointer();
			Assert::AreEqual(std::size_t(10), StandardAllocatorTestComponent::GetStorage().size());
			anotherComponent->Value = 70;
			Assert::AreEqual(70, anotherComponent->Value);
		}
	};
}