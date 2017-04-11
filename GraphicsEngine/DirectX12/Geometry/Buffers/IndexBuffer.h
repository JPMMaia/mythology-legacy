#pragma once

#include "BaseGeometryBuffer.h"

namespace GraphicsEngine
{
	class IndexBuffer : public BaseGeometryBuffer
	{
	public:
		IndexBuffer() = default;
		IndexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const void* indices, size_t indexCount, size_t indexByteSize, DXGI_FORMAT indexFormat);
		IndexBuffer(const IndexBuffer& rhs) = default;
		IndexBuffer& operator=(const IndexBuffer& rhs) = default;

		const D3D12_INDEX_BUFFER_VIEW& View() const;
		DXGI_FORMAT GetIndexFormat() const;

	private:
		DXGI_FORMAT m_indexFormat = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
		D3D12_INDEX_BUFFER_VIEW m_view;
	};
}
