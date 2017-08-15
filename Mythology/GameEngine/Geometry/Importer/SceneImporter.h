#pragma once

#include "GameEngine/Geometry/EigenGeometry.h"

#include <deque>

struct aiMesh;

namespace GameEngine
{
	class SceneImporter
	{
	public:
		using VertexType = EigenVertex;
		using MeshDataType = EigenMeshData;

	public:
		struct Geometry
		{
			std::string Name;
			MeshDataType MeshData;
		};
		struct ImportedScene
		{
			std::deque<Geometry> Geometries;
		};

	public:
		static void Import(const std::wstring& filePath, ImportedScene& importedScene);

	private:
		static MeshDataType CreateMeshData(const aiMesh& mesh);
	};
}
