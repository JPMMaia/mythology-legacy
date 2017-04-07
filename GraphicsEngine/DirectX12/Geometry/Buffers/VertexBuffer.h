#pragma once

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

		uint32_t GetVertexByteStride() const;

	private:
		uint32_t m_vertexByteStride = 0;
	};
}
