﻿#pragma once

#include "BaseGeometryBuffer.h"

namespace GraphicsEngine
{
	class VertexBuffer : public BaseGeometryBuffer
	{
	public:
		VertexBuffer() = default;
		explicit VertexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, const void* vertices, size_t vertexCount, size_t vertexByteSize);
		VertexBuffer(const VertexBuffer& rhs) = default;
		VertexBuffer& operator=(const VertexBuffer& rhs) = default;

		const D3D12_VERTEX_BUFFER_VIEW& View() const;
		uint32_t VertexByteStride() const;

	private:
		uint32_t m_vertexByteStride = 0;
		D3D12_VERTEX_BUFFER_VIEW m_view;
	};
}
