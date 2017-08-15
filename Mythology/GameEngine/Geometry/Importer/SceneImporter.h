#pragma once

#include "GameEngine/Geometry/EigenGeometry.h"

#include <deque>

struct aiMaterial;
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
			std::size_t MaterialIndex;
		};
		struct Material
		{
			
		};
		struct ImportedScene
		{
			std::deque<Geometry> Geometries;
			std::deque<Material> Materials;
		};

	public:
		static void Import(const std::wstring& filePath, ImportedScene& importedScene);

	private:
		static MeshDataType CreateMeshData(const aiMesh& mesh);
		static Material CreateMaterial(const aiMaterial& material);
	};
}
