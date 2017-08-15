#include "pch.h"
#include "CppUnitTest.h"
#include "Core/DeviceResources.h"
#include "Core/Resources/GPUAllocator.h"
#include "Core/Shader/ShaderBufferTypes.h"

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

		TEST_METHOD(GPUAllocatorTest3)
		{
			auto deviceResources = std::make_shared<DeviceResources>();

			GPUUploadBuffer<ShaderBufferTypes::MaterialData> materials(GPUAllocator<float>(deviceResources->GetD3DDevice(), false));

			materials.reserve(4);
			Assert::AreEqual(std::size_t(4), materials.capacity());
			Assert::AreEqual(std::size_t(0), materials.size());

			// Add first material:
			{
				ShaderBufferTypes::MaterialData material = {};
				material.BaseColor = { 1.0f, 1.0f, 1.0f, 1.0f };
				material.AlbedoMapIndex = 1;
				materials.emplace_back(std::move(material));
			}
			Assert::AreEqual(std::size_t(4), materials.capacity());
			Assert::AreEqual(std::size_t(1), materials.size());
			Assert::IsTrue(Vector4f(1.0f, 1.0f, 1.0f, 1.0f) == materials[0].BaseColor);
			Assert::AreEqual(std::uint32_t(1), materials[0].AlbedoMapIndex);

			// Add second material:
			{
				ShaderBufferTypes::MaterialData material = {};
				material.BaseColor = { 1.0f, 0.0f, 0.0f, 1.0f };
				material.AlbedoMapIndex = 0;
				materials.emplace_back(material);
			}
			Assert::AreEqual(std::size_t(4), materials.capacity());
			Assert::AreEqual(std::size_t(2), materials.size());
			Assert::IsTrue(Vector4f(1.0f, 0.0f, 0.0f, 1.0f) == materials[1].BaseColor);
			Assert::AreEqual(std::uint32_t(0), materials[1].AlbedoMapIndex);

			// Add third material:
			{
				ShaderBufferTypes::MaterialData material = {};
				material.BaseColor = { 0.0f, 1.0f, 0.0f, 1.0f };
				material.AlbedoMapIndex = 0;
				materials.push_back(std::move(material));
			}
			Assert::AreEqual(std::size_t(4), materials.capacity());
			Assert::AreEqual(std::size_t(3), materials.size());
			Assert::IsTrue(Vector4f(0.0f, 1.0f, 0.0f, 1.0f) == materials[2].BaseColor);
			Assert::AreEqual(std::uint32_t(0), materials[2].AlbedoMapIndex);

			// Add fourth material:
			{
				ShaderBufferTypes::MaterialData material = {};
				material.BaseColor = { 0.0f, 0.0f, 1.0f, 1.0f };
				material.AlbedoMapIndex = 0;
				materials.push_back(material);
			}
			Assert::AreEqual(std::size_t(4), materials.capacity());
			Assert::AreEqual(std::size_t(4), materials.size());
			Assert::IsTrue(Vector4f(0.0f, 0.0f, 1.0f, 1.0f) == materials[3].BaseColor);
			Assert::AreEqual(std::uint32_t(0), materials[3].AlbedoMapIndex);
		}
    };
}