#pragma once

#include "IMesh.h"
#include "Core/Geometry/Buffers/VertexBuffer.h"
#include "Core/Geometry/Buffers/IndexBuffer.h"
#include "Core/Geometry/Submesh.h"

#include <unordered_map>

namespace DirectX12Engine
{
	class ImmutableMesh : public IMesh
	{
	public:
		explicit ImmutableMesh(const std::string& name, VertexBuffer&& vertexBuffer, IndexBuffer&& indexBuffer);

		void AddSubmesh(const std::string& name, const Submesh& submesh);

		void Render(ID3D12GraphicsCommandList* commandList, const std::string& submeshName) const;

		void DisposeUploadBuffers() override;

		const std::string& Name() const;

	private:
		std::string m_name;
		std::unordered_map<std::string, Submesh> m_submeshes;
		VertexBuffer m_vertexBuffer;
		IndexBuffer m_indexBuffer;
	};
}
