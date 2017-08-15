#include "SceneImporter.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Common/Helpers.h"

using namespace Assimp;
using namespace Common;
using namespace GameEngine;

void SceneImporter::Import(const std::wstring& filePath, ImportedScene& importedScene)
{
	Importer importer;

	auto flags =
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_GenNormals |
		aiProcess_SortByPType;
	auto scene = importer.ReadFile(Helpers::WStringToString(filePath), flags);

	if (!scene)
		throw std::invalid_argument("Can't import given model.");

	for (std::size_t i = 0; i < scene->mNumMeshes; ++i)
	{
		auto mesh = scene->mMeshes[i];

		Geometry geometry;
		geometry.Name = mesh->mName.C_Str();
		geometry.MeshData = CreateMeshData(*mesh);
		geometry.MaterialIndex = mesh->mMaterialIndex;
		importedScene.Geometries.emplace_back(std::move(geometry));
	}

	for(std::size_t i = 0; i < scene->mNumMaterials; ++i)
	{
		importedScene.Materials.emplace_back(scene->mMaterials[i]);
	}
}

SceneImporter::MeshDataType SceneImporter::CreateMeshData(const aiMesh& mesh)
{
	MeshDataType meshData;

	{
		auto& vertices = meshData.Vertices;
		vertices.resize(mesh.mNumVertices);

		for(std::size_t i = 0; i < mesh.mNumVertices; ++i)
		{
			const auto& position = mesh.mVertices[i];
			vertices[i].Position = { position.x, position.y, position.z };
		}

		for (std::size_t i = 0; i < mesh.mNumVertices; ++i)
		{
			const auto& normal = mesh.mNormals[i];
			vertices[i].Normal = { normal.x, normal.y, normal.z };
		}

		if (mesh.HasTextureCoords(0))
		{
			for (std::size_t i = 0; i < mesh.mNumVertices; ++i)
			{
				const auto& tangent = mesh.mTangents[i];
				vertices[i].Tangent = { tangent.x, tangent.y, tangent.z };
			}

			for (std::size_t i = 0; i < mesh.mNumVertices; ++i)
			{
				const auto& textureCoordinates = mesh.mTextureCoords[0][i];
				vertices[i].TextureCoordinates = { textureCoordinates.x, textureCoordinates.y };
			}	
		}
			
	}
	
	{
		meshData.Indices.reserve(mesh.mNumFaces * 3);
		for (std::size_t i = 0; i < mesh.mNumFaces; ++i)
		{
			const auto& faceIndices = mesh.mFaces->mIndices;

			meshData.Indices.emplace_back(faceIndices[0]);
			meshData.Indices.emplace_back(faceIndices[1]);
			meshData.Indices.emplace_back(faceIndices[2]);
		}
	}
		
	return meshData;
}

SceneImporter::Material SceneImporter::CreateMaterial(const aiMaterial& material)
{
	Material data;

	return data;
}
