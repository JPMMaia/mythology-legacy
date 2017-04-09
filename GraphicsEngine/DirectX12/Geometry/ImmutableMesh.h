#pragma once

#include "Interfaces/IGeometry.h"
#include "DirectX12/Geometry/Buffers/VertexBuffer.h"
#include "DirectX12/Geometry/Buffers/IndexBuffer.h"
#include "DirectX12/Geometry/Submesh.h"

#include <unordered_map>
#include "MeshGenerator.h"

namespace GraphicsEngine
{
	class ImmutableMesh : public IGeometry
	{
	public:
		explicit ImmutableMesh(const std::string& name, const MeshGenerator::MeshData& meshData);

		void AddSubmesh(const std::string& name, const Submesh& submesh);

	private:
		std::string m_name;
		std::unordered_map<std::string, Submesh> m_submeshes;
		VertexBuffer m_vertexBuffer;
		IndexBuffer m_indexBuffer;
	};
}
