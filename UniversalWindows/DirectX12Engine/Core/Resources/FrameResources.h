#pragma once

#include "GPUAllocator.h"

namespace DirectX12Engine
{
	template<class DataType>
	class FrameResources
	{
	public:

		D3D12_GPU_VIRTUAL_ADDRESS GetCurrentFrameBufferGPUVirtualAddress()
		{
			return m_data.get_allocator().GetGPUVirtualAddress(0);
		}

	private:
		std::vector<DataType, GPUAllocator<DataType>> m_data;
	};
}
