#include "pch.h"
#include "CppUnitTest.h"
#include "Core/DeviceResources.h"
#include "Core/Resources/GPUAllocator.h"

#include <algorithm>
#include <numeric>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace DirectX12Engine;

namespace DirectX12TestProject
{
    TEST_CLASS(GPUAllocatorTests)
    {
    public:
		TEST_METHOD(GPUAllocatorTest1)
		{
			auto deviceResources = std::make_shared<DeviceResources>();

			GPUAllocator<float> allocator(deviceResources->GetD3DDevice());

			auto* pointer = allocator.allocate(4);

			pointer[0] = 1.0f;
			pointer[1] = 2.0f;
			pointer[2] = 3.0f;
			pointer[3] = 4.0f;

			allocator.deallocate(pointer, 4);
		}

		TEST_METHOD(GPUAllocatorTest2)
		{
			auto deviceResources = std::make_shared<DeviceResources>();

			//std::vector<float, GPUAllocator<float>> numbers(4, GPUAllocator<float>(deviceResources->GetD3DDevice()));
			std::vector<float> numbers(4);

			std::iota(numbers.begin(), numbers.end(), 1.0f);

			numbers.clear();
			numbers.shrink_to_fit();
		}
    };
}