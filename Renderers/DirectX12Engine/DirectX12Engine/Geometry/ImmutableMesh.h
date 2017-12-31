#pragma once

#include "IMesh.h"
#include "DirectX12Engine/Geometry/Buffers/VertexBuffer.h"
#include "DirectX12Engine/Geometry/Buffers/IndexBuffer.h"
#include "DirectX12Engine/Geometry/Submesh.h"

#include <unordered_map>

namespace DirectX12Engine
{
	class ImmutableMesh : public IMesh
	{
	public:
		explicit ImmutableMesh(const std::string& name, VertexBuffer&& vertexBuffer, IndexBuffer&& indexBuffer);

		void AddSubmesh(const std::string& name, const Submesh& submesh);

		void Render(ID3D12GraphicsCommandList* commandList, const std::string& submeshName, UINT instanceCount) const;

		const std::string& Name() const;

	private:
		std::string m_name;
		std::unordered_map<std::string, Submesh> m_submeshes;
		VertexBuffer m_vertexBuffer;
		IndexBuffer m_indexBuffer;
	};
}
