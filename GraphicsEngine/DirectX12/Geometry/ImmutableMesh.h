#pragma once

#include "Interfaces/IGeometry.h"
#include "DirectX12/Geometry/Buffers/VertexBuffer.h"
#include "DirectX12/Geometry/Buffers/IndexBuffer.h"
#include "DirectX12/Geometry/SubMesh.h"

#include <unordered_map>

namespace GraphicsEngine
{
	class ImmutableMesh : public IGeometry
	{
	public:
		ImmutableMesh(VertexBuffer vertexBuffer, IndexBuffer indexBuffer);

	private:
		std::string m_name;
		std::unordered_map<std::string, SubMesh> m_subMeshes;
		VertexBuffer m_vertexBuffer;
		IndexBuffer m_indexBuffer;
	};
}
