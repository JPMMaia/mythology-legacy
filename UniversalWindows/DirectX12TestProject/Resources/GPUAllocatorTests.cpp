#include "pch.h"
#include "UnitTestApp.xaml.h"
#include "CppUnitTest.h"
#include "Core/DeviceResources.h"
#include "Core/Resources/GPUAllocator.h"

#include <algorithm>
#include <numeric>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace DirectX12Engine;
using namespace Eigen;

namespace DirectX12TestProject
{
    TEST_CLASS(GPUAllocatorTests)
    {
    public:
		TEST_METHOD(GPUAllocatorTest1)
		{
			auto deviceResources = std::make_shared<DeviceResources>();

			GPUAllocator<float> allocator(deviceResources->GetD3DDevice(), false);

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

			GPUUploadBuffer<float> numbers(4, GPUAllocator<float>(deviceResources->GetD3DDevice(), false));

			std::iota(numbers.begin(), numbers.end(), 1.0f);

			Assert::AreEqual(1.0f, numbers[0]);
			Assert::AreEqual(2.0f, numbers[1]);
			Assert::AreEqual(3.0f, numbers[2]);
			Assert::AreEqual(4.0f, numbers[3]);

			numbers.clear();
			Assert::AreEqual(std::size_t(0), numbers.size());

			numbers.shrink_to_fit();
			Assert::AreEqual(std::size_t(0), numbers.capacity());
		}
    };
}