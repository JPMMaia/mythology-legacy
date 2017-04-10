#pragma once

#include "Interfaces/IMesh.h"
#include "DirectX12/Geometry/Buffers/VertexBuffer.h"
#include "DirectX12/Geometry/Buffers/IndexBuffer.h"
#include "DirectX12/Geometry/Submesh.h"

#include <unordered_map>
#include "MeshGenerator.h"

namespace GraphicsEngine
{
	class ImmutableMesh : public IMesh
	{
	public:
		explicit ImmutableMesh(const std::string& name, VertexBuffer&& vertexBuffer, IndexBuffer&& indexBuffer);

		void AddSubmesh(const std::string& name, const Submesh& submesh);

		const std::string& Name() const;



	private:
		std::string m_name;
		std::unordered_map<std::string, Submesh> m_submeshes;
		VertexBuffer m_vertexBuffer;
		IndexBuffer m_indexBuffer;
	};
}
