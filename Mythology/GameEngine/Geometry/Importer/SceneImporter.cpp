#include "SceneImporter.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Common/Helpers.h"

#include <iterator>

using namespace Assimp;
using namespace Common;
using namespace GameEngine;

std::ostream& GameEngine::operator<<(std::ostream& outputStream, const SceneImporter::Geometry& geometry)
{
	outputStream << geometry.Name << " ";
	outputStream << geometry.MaterialIndex << " ";

	{
		const auto& vertices = geometry.MeshData.Vertices;

		outputStream << vertices.size() << " ";
		std::for_each(vertices.begin(), vertices.end(), [&outputStream](const auto& vertex)
		{
			const auto& position = vertex.Position;
			outputStream << position[0] << " " << position[1] << " " << position[2] << " ";

			const auto& normal = vertex.Normal;
			outputStream << normal[0] << " " << normal[1] << " " << normal[2] << " ";

			const auto& tangent = vertex.Tangent;
			outputStream << tangent[0] << " " << tangent[1] << " " << tangent[2] << " ";

			const auto& textureCoordinates = vertex.TextureCoordinates;
			outputStream << textureCoordinates[0] << " " << textureCoordinates[1] << " ";
		});
	}

	{
		const auto& indices = geometry.MeshData.Indices;

		outputStream << indices.size() << " ";
		std::copy(indices.begin(), indices.end(), std::ostream_iterator<EigenMeshData::IndexType>(outputStream, " "));
	}

	return outputStream;
}
std::istream& GameEngine::operator>>(std::istream& inputStream, SceneImporter::Geometry& geometry)
{
	inputStream >> geometry.Name;
	inputStream >> geometry.MaterialIndex;

	{
		auto& vertices = geometry.MeshData.Vertices;

		std::size_t vertexCount;
		inputStream >> vertexCount;

		vertices.resize(vertexCount);
		std::for_each(vertices.begin(), vertices.end(), [&inputStream](auto& vertex)
		{
			auto& position = vertex.Position;
			inputStream >> position[0] >> position[1] >> position[2];

			auto& normal = vertex.Normal;
			inputStream >> normal[0] >> normal[1] >> normal[2];

			auto& tangent = vertex.Tangent;
			inputStream >> tangent[0] >> tangent[1] >> tangent[2];

			auto& textureCoordinates = vertex.TextureCoordinates;
			inputStream >> textureCoordinates[0] >> textureCoordinates[1];
		});
	}

	{
		auto& indices = geometry.MeshData.Indices;

		std::size_t indexCount;
		inputStream >> indexCount;

		indices.reserve(indexCount);
		std::copy_n(std::istream_iterator<EigenMeshData::IndexType>(inputStream), indexCount, std::back_inserter(indices));
	}

	return inputStream;
}

std::ostream& GameEngine::operator<<(std::ostream& outputStream, const SceneImporter::Material& material)
{
	auto outputMap = [&outputStream](const auto& map)
	{
		auto outputContainer = [&outputStream](const auto& container)
		{
			outputStream << container.size() << " ";
			std::for_each(container.begin(), container.end(), [&outputStream](const auto& element)
			{
				outputStream << element << " ";
			});
		};

		outputStream << map.size() << " ";
		std::for_each(map.begin(), map.end(), [&outputStream, &outputContainer](const auto& map)
		{
			outputStream << map.first << " ";
			outputContainer(map.second);
		});
	};

	outputMap(material.IntegerProperties);
	outputMap(material.FloatProperties);
	outputMap(material.DoubleProperties);
	
	{
		const auto& map = material.StringProperties;
		outputStream << map.size() << " ";
		std::for_each(map.begin(), map.end(), [&outputStream](const auto& map)
		{
			outputStream << map.first << " " << map.second << " ";
		});
	}

	outputStream << material.DiffuseTexturePath << " ";

	return outputStream;
}

template <class KeyType, class ValueType, class MapType>
void GameEngine::InputMap(std::istream& inputStream, MapType& map)
{
	auto inputContainer = [&inputStream](auto& container)
	{
		std::size_t elementCount;
		inputStream >> elementCount;

		container.resize(elementCount);
		std::for_each(container.begin(), container.end(), [&inputStream](auto& element)
		{
			inputStream >> element;
		});
	};

	std::size_t elementCount;
	inputStream >> elementCount;

	for (std::size_t i = 0; i < elementCount; ++i)
	{
		KeyType key;
		inputStream >> key;

		ValueType value;
		inputContainer(value);

		map.emplace(key, value);
	}
}
std::istream& GameEngine::operator>>(std::istream& inputStream, SceneImporter::Material& material)
{
	InputMap<std::string, std::vector<std::int32_t>>(inputStream, material.IntegerProperties);
	InputMap<std::string, std::vector<float>>(inputStream, material.FloatProperties);
	InputMap<std::string, std::vector<double>>(inputStream, material.DoubleProperties);
	
	{
		auto& map = material.StringProperties;

		std::size_t elementCount;
		inputStream >> elementCount;

		for(std::size_t i = 0; i < elementCount; ++i)
		{
			std::string key, value;
			inputStream >> key >> value;
			map.emplace(key, value);
		}
	}

	inputStream >> material.DiffuseTexturePath;

	return inputStream;
}

std::ostream& GameEngine::operator<<(std::ostream& outputStream, const SceneImporter::ImportedScene& importedScene)
{
	{
		const auto& geometries = importedScene.Geometries;

		// Output number of geometries:
		outputStream << geometries.size() << " ";

		// Output each geometry:
		std::for_each(geometries.begin(), geometries.end(), [&outputStream](const auto& geometry)
		{
			outputStream << geometry;
		});
	}

	{
		const auto& materials = importedScene.Materials;

		// Output number of materials:
		outputStream << materials.size() << " ";

		// Output each material:
		std::for_each(materials.begin(), materials.end(), [&outputStream](const auto& material)
		{
			outputStream << material;
		});
	}

	return outputStream;
}
std::istream& GameEngine::operator>>(std::istream& inputStream, SceneImporter::ImportedScene& importedScene)
{
	{
		auto& geometries = importedScene.Geometries;

		// Read number of geometries:
		std::size_t geometryCount;
		inputStream >> geometryCount;

		// Read geometries:
		geometries.resize(geometryCount);
		for (std::size_t i = 0; i < geometryCount; ++i)
			inputStream >> geometries[i];
	}

	{
		auto& materials = importedScene.Materials;

		// Read number of materials:
		std::size_t materialCount;
		inputStream >> materialCount;

		// Read materials:
		materials.resize(materialCount);
		for (std::size_t i = 0; i < materialCount; ++i)
			inputStream >> materials[i];
	}

	return inputStream;
}

void SceneImporter::Import(const std::wstring& filePath, ImportedScene& importedScene)
{
	Importer importer;

	auto flags =
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_GenNormals |
		aiProcess_GenUVCoords |
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

	for (std::size_t i = 0; i < scene->mNumMaterials; ++i)
	{
		importedScene.Materials.emplace_back(CreateMaterial(*scene->mMaterials[i]));
	}
}

SceneImporter::MeshDataType SceneImporter::CreateMeshData(const aiMesh& mesh)
{
	MeshDataType meshData;

	{
		auto& vertices = meshData.Vertices;
		vertices.resize(mesh.mNumVertices);

		for (std::size_t i = 0; i < mesh.mNumVertices; ++i)
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
			const auto& faceIndices = mesh.mFaces[i].mIndices;

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

	aiString texturePath;
	material.GetTexture(aiTextureType_DIFFUSE, 0, &texturePath);
	data.DiffuseTexturePath = texturePath.C_Str();

	for (std::size_t i = 0; i < material.mNumProperties; ++i)
	{
		auto property = material.mProperties[i];

		switch (property->mType)
		{
		case aiPTI_Float:
			data.FloatProperties[property->mKey.C_Str()] = ParseArray<std::vector<float>, float>(*property);
			break;
		case aiPTI_Double:
			data.DoubleProperties[property->mKey.C_Str()] = ParseArray<std::vector<double>, double>(*property);
			break;
		case aiPTI_String:
			data.StringProperties[property->mKey.C_Str()] = ParseArray<std::string, char>(*property);
			break;
		case aiPTI_Integer:
			data.IntegerProperties[property->mKey.C_Str()] = ParseArray<std::vector<std::int32_t>, std::int32_t>(*property);
			break;

		default:
			break;
		}
	}

	return data;
}

template <typename ContainerType, typename DataType>
ContainerType SceneImporter::ParseArray(const aiMaterialProperty& property)
{
	auto count = static_cast<std::size_t>(property.mDataLength / sizeof(DataType));

	ContainerType container;
	container.resize(count);
	std::memcpy(reinterpret_cast<void*>(&container[0]), property.mData, property.mDataLength);
	return container;
}
