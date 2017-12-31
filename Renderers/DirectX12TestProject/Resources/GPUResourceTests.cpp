#include "pch.h"
#include "UnitTestApp.xaml.h"
#include "CppUnitTest.h"
#include "DirectX12Engine/DeviceResources.h"
#include "DirectX12Engine/Resources/GPUResource.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace DirectX12Engine;
using namespace Eigen;

namespace DirectX12TestProject
{
	struct TestResource
	{
	public:
		std::size_t Value0;
		float Value1;

	public:
		TestResource() :
			Value0(0),
			Value1(0.0f)
		{
		}

		TestResource(std::size_t value0, float value1) :
			Value0(value0),
			Value1(value1)
		{
		}

	public:
		bool operator==(const TestResource& rhs) const
		{
			return Value0 == rhs.Value0 && Value1 == rhs.Value1;
		}
	};

	TEST_CLASS(GPUResourceTests)
	{
	private:
		static std::shared_ptr<DeviceResources> s_deviceResources;
		static constexpr std::size_t c_frameCount = 3;

	public:
		// Constructor initializes default values.
		TEST_METHOD(Constructor)
		{
			GPUResource<TestResource> resources(s_deviceResources, false, c_frameCount);
			Assert::AreEqual(std::size_t(0), resources.GetCapacity());
			Assert::AreEqual(std::size_t(0), resources.GetSize());
		}

		// Add an element at the first vacant position of the container.
		TEST_METHOD(Push)
		{
			GPUResource<TestResource> resources(s_deviceResources, false, c_frameCount);

			TestResource resource0(3, 4.0f);
			auto index0 = resources.Push(resource0);
			Assert::AreEqual(std::size_t(1), resources.GetSize());

			auto index1 = resources.Push(TestResource(1, 2.0f));
			Assert::AreEqual(std::size_t(2), resources.GetSize());

			auto index2 = resources.Push(resource0);
			Assert::AreEqual(std::size_t(3), resources.GetSize());

			Assert::IsTrue(resource0 == resources.Get(index0));
			Assert::IsTrue(TestResource(1, 2.0f) == resources.Get(index1));
			Assert::IsTrue(resource0 == resources.Get(index2));
		}

		// Construct an element in-place at the first available position of the container.
		TEST_METHOD(Emplace)
		{
			GPUResource<TestResource> resources(s_deviceResources, false, c_frameCount);

			auto index0 = resources.Emplace();
			Assert::AreEqual(std::size_t(1), resources.GetSize());

			auto index1 = resources.Emplace(3, 4.0f);
			Assert::AreEqual(std::size_t(2), resources.GetSize());

			Assert::IsTrue(TestResource(0, 0.0f) == resources.Get(index0));
			Assert::IsTrue(TestResource(3, 4.0f) == resources.Get(index1));
		}

		// Erase an element at an arbitrary position.
		// The element is swapped with the back and erased. 
		// The function returns the index of the previous back element.
		TEST_METHOD(SwapWithBackAndErase)
		{
			GPUResource<TestResource> resources(s_deviceResources, false, c_frameCount);

			auto index0 = resources.Emplace(0, 0.0f);
			resources.Emplace(1, 1.0f);
			resources.Emplace(2, 2.0f);
			resources.SwapWithBackAndErase(index0);

			Assert::AreEqual(std::size_t(2), resources.GetSize());
			Assert::IsTrue(TestResource(2, 2.0f) == resources.Get(index0));
		}

		// Update the value of an element at an arbitrary position that is initialized.
		TEST_METHOD(Set)
		{
			GPUResource<TestResource> resources(s_deviceResources, false, c_frameCount);

			resources.Emplace(0, 0.0f);
			auto index1 = resources.Emplace(1, 1.0f);
			auto index2 = resources.Emplace(2, 2.0f);

			Assert::IsTrue(TestResource(1, 1.0f) == resources.Get(index1));
			Assert::IsTrue(TestResource(2, 2.0f) == resources.Get(index2));

			TestResource resource0(3, 3.0f);
			resources.Set(index1, resource0);
			resources.Set(index2, std::move(resource0));
			Assert::IsTrue(TestResource(3, 3.0f) == resources.Get(index1));
			Assert::IsTrue(TestResource(3, 3.0f) == resources.Get(index2));
		}

		// Reduce memory usage by freeing unused memory.
		TEST_METHOD(ShrinkToFit)
		{
			GPUResource<TestResource> resources(s_deviceResources, false, c_frameCount);

			for (std::size_t i = 0; i < 6; ++i)
				resources.Emplace();

			resources.ShrinkToFit();
			Assert::AreEqual(std::size_t(6), resources.GetCapacity());

			resources.SwapWithBackAndErase(0);
			resources.ShrinkToFit();
			Assert::AreEqual(std::size_t(5), resources.GetCapacity());

			// Erase at the end:
			resources.SwapWithBackAndErase(3);
			resources.SwapWithBackAndErase(3);
			resources.ShrinkToFit();
			Assert::AreEqual(std::size_t(3), resources.GetCapacity());
		}

		// By calling update, the GPU upload buffer is updated with the local changes.
		TEST_METHOD(Update)
		{
			GPUResource<TestResource> resources(s_deviceResources, false, c_frameCount);

			auto copyDataFromGPU = [](ID3D12Resource* gpuBuffer, std::size_t capacity)
			{
				unsigned char* m_mappedData;

				CD3DX12_RANGE readRange(0, 0);
				gpuBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_mappedData));

				std::vector<TestResource> data(capacity);
				std::memcpy(data.data(), m_mappedData, capacity * sizeof(TestResource));

				gpuBuffer->Unmap(0, &readRange);

				return data;
			};

			resources.Resize(10, TestResource(1, 2.0f));
			resources.Update(0);
			{
				auto data = copyDataFromGPU(resources.GetGPUBuffer(0), resources.GetCapacity());

				TestResource expectedValue(1, 2.0f);
				for (const auto& element : data)
					Assert::IsTrue(expectedValue == element);
			}

			resources.SwapWithBackAndErase(4);
			resources.Set(6, TestResource(4, 5.0f));
			resources.Update(1);
			{
				auto data = copyDataFromGPU(resources.GetGPUBuffer(1), resources.GetCapacity());
				Assert::IsTrue(TestResource(4, 5.0f) == data.at(6));
			}

			resources.Update(2);
			{
				auto data = copyDataFromGPU(resources.GetGPUBuffer(2), resources.GetCapacity());
				Assert::IsTrue(TestResource(4, 5.0f) == data.at(6));
			}

			resources.Update(0);
			{
				auto data = copyDataFromGPU(resources.GetGPUBuffer(0), resources.GetCapacity());
				Assert::IsTrue(TestResource(4, 5.0f) == data.at(6));
			}
		}
	};
	std::shared_ptr<DeviceResources> GPUResourceTests::s_deviceResources = std::make_shared<DeviceResources>();
}