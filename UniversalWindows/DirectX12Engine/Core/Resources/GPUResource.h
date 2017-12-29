#pragma once

#include "Core/DeviceResources.h"
#include "Core/Utilities/UnmappedUploadBuffer.h"

#include <cstddef>
#include <memory>
#include <set>
#include <vector>

namespace DirectX12Engine
{
	template<class Type>
	class GPUResource
	{
	public:
		GPUResource(const std::shared_ptr<DeviceResources>& deviceResources, bool isConstantBuffer, std::size_t frameCount) :
			m_dirtyFlags(frameCount, false)
		{
			m_uploadBuffers.reserve(frameCount);
			for (std::size_t i = 0; i < frameCount; ++i)
				m_uploadBuffers.emplace_back(deviceResources, isConstantBuffer);
		}

	public:
		void Update(std::size_t frameIndex)
		{
			auto& isDirty = m_dirtyFlags[frameIndex];
			if (!isDirty)
				return;

			auto& uploadBuffer = m_uploadBuffers[frameIndex];

			auto cpuBufferCapacity = GetCapacity();
			if (uploadBuffer.GetElementCount() != cpuBufferCapacity)
			{
				uploadBuffer.SetCapacity(cpuBufferCapacity);
			}

			auto size = m_localData.size();
			if(size > 0)
				uploadBuffer.CopyData(m_localData.data(), 0, size);

			isDirty = false;
		}

	public:
		std::size_t Push(const Type& data)
		{
			return Emplace(data);
		}
		std::size_t Push(Type&& data)
		{
			return Emplace(std::move(data));
		}

		template<class... Arguments>
		std::size_t Emplace(Arguments&&... arguments)
		{
			SetDirtyFlags();

			m_localData.emplace_back(std::forward<Arguments>(arguments)...);
			return m_localData.size() - 1;
		}

	public:
		bool SwapWithBackAndErase(std::size_t index)
		{
			if (index == m_localData.size() - 1)
			{
				m_localData.pop_back();
				return false;
			}

			std::swap(m_localData[index], m_localData.back());
			m_localData.pop_back();
			return true;
		}

	public:
		const Type& Get(std::size_t index) const
		{
			return m_localData.at(index);
		}
		void Set(std::size_t index, const Type& data)
		{
			auto copy = data;
			Set(index, std::move(copy));
		}
		void Set(std::size_t index, Type&& data)
		{
			SetDirtyFlags();

			m_localData[index] = std::move(data);
		}

	public:
		void Reserve(std::size_t capacity)
		{
			SetDirtyFlags();

			m_localData.reserve(capacity);
		}
		void Resize(std::size_t size, const Type& data = Type())
		{
			SetDirtyFlags();

			m_localData.resize(size, data);
		}
		void ShrinkToFit()
		{
			SetDirtyFlags();

			m_localData.shrink_to_fit();
		}

	public:
		std::size_t GetCapacity() const
		{
			return m_localData.capacity();
		}
		std::size_t GetSize() const
		{
			return m_localData.size();
		}
		bool IsEmpty() const
		{
			return m_localData.size() == 0;
		}

	public:
		ID3D12Resource* GetGPUBuffer(std::size_t frameIndex) const
		{
			return m_uploadBuffers.at(frameIndex).GetResource();
		}
		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress(std::size_t frameIndex, std::uint32_t index) const
		{
			return m_uploadBuffers.at(frameIndex).GetGPUVirtualAddress(index);
		}

	private:
		void SetDirtyFlags() const
		{
			std::fill(m_dirtyFlags.begin(), m_dirtyFlags.end(), true);
		}

	private:
		std::vector<UnmappedUploadBuffer<Type>> m_uploadBuffers;
		mutable std::vector<bool> m_dirtyFlags;
		std::vector<Type> m_localData;
	};
}
