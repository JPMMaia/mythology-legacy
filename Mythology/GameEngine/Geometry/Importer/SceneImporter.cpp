#include "SceneImporter.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Common/Helpers.h"

#include <iterator>
#include <unordered_set>
#include "GameEngine/Geometry/Animation/AnimationClip.h"
#include <queue>

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

		for (std::size_t i = 0; i < elementCount; ++i)
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
		aiProcess_FindInvalidData |
		aiProcess_JoinIdenticalVertices |
		aiProcess_LimitBoneWeights |
		aiProcess_ImproveCacheLocality |
		aiProcess_ValidateDataStructure |
		aiProcess_FlipUVs |
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

	{
		// Create skeleton:
		auto skeleton = CreateSkeleton(*scene);

		// Add bone weights and indices to the mesh data:
		for (std::size_t i = 0; i < scene->mNumMeshes; ++i)
		{
			auto mesh = scene->mMeshes[i];
			auto& geometry = importedScene.Geometries[i];

			AddBoneData(skeleton, *mesh, geometry);
		}

		// Create animation clips:
		std::unordered_map<std::string, AnimationClip> animations;
		for (std::size_t i = 0; i < scene->mNumAnimations; ++i)
		{
			auto animationData = scene->mAnimations[i];
			animations.emplace(animationData->mName.C_Str(), CreateSkinnedAnimation(*animationData, skeleton));
		}

		importedScene.SkinnedData = SkinnedData(skeleton.BoneHierarchy, skeleton.BoneOffsets, animations);
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

AnimationClip SceneImporter::CreateSkinnedAnimation(const aiAnimation& animationData, const Skeleton& skeleton)
{
	std::vector<BoneAnimation> boneAnimations;
	boneAnimations.resize(static_cast<std::size_t>(animationData.mNumChannels));
	for (std::size_t channelIndex = 0; channelIndex < animationData.mNumChannels; ++channelIndex)
	{
		auto channel = animationData.mChannels[channelIndex];

		// Do not add animations that do not refer to a bone:
		if (std::find(skeleton.Bones.begin(), skeleton.Bones.end(), channel->mNodeName.C_Str()) == skeleton.Bones.end())
			continue;

		std::vector<Keyframe> keyframes(channel->mNumPositionKeys);
		for (std::size_t keyIndex = 0; keyIndex < keyframes.size(); ++keyIndex)
		{
			{
				const auto& key = channel->mPositionKeys[keyIndex];
				keyframes[keyIndex].TimePosition = static_cast<float>(key.mTime);
				keyframes[keyIndex].Translation = { key.mValue.x, key.mValue.y, key.mValue.z };
			}

			{
				const auto& key = channel->mRotationKeys[keyIndex];
				keyframes[keyIndex].Rotation = { key.mValue.x, key.mValue.y, key.mValue.z, key.mValue.w };
			}

			{
				const auto& key = channel->mScalingKeys[keyIndex];
				keyframes[keyIndex].Scaling = { key.mValue.x, key.mValue.y, key.mValue.z };
			}
		}

		boneAnimations[channelIndex] = BoneAnimation(keyframes);
	}

	return AnimationClip(boneAnimations);
}

SceneImporter::Skeleton SceneImporter::CreateSkeleton(const aiScene& scene)
{
	Skeleton skeleton;

	// Find all bones:
	std::unordered_set<std::string> boneNames;
	std::unordered_map<std::string, Eigen::Affine3f> boneOffsets;
	for (std::size_t meshIndex = 0; meshIndex < scene.mNumMeshes; ++meshIndex)
	{
		auto mesh = scene.mMeshes[meshIndex];

		for (std::size_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
		{
			auto bone = mesh->mBones[boneIndex];
			boneNames.emplace(bone->mName.C_Str());

			Eigen::Matrix4f offsetMatrix;
			{
				const auto& m = bone->mOffsetMatrix;
				offsetMatrix <<
					m.a1, m.a2, m.a3, m.a4,
					m.b1, m.b2, m.b3, m.b4,
					m.c1, m.c2, m.c3, m.c4,
					m.d1, m.d2, m.d3, m.d4;
			}

			boneOffsets.emplace(bone->mName.C_Str(), Eigen::Affine3f(offsetMatrix));
		}
	}

	// Find the node of each bone:
	std::unordered_map<std::string, aiNode*> boneNodes;
	for (const auto& boneName : boneNames)
		boneNodes.emplace(boneName, scene.mRootNode->FindNode(boneName.c_str()));

	// Find the root node of the skeleton:
	auto lowestDistance = (std::numeric_limits<std::size_t>::max)();
	std::string rootNodeName;
	for (const auto& boneNode : boneNodes)
	{
		std::size_t distance = 0;

		for (auto currentNode = boneNode.second->mParent; currentNode != nullptr; currentNode = currentNode->mParent)
			++distance;

		if (distance < lowestDistance)
		{
			rootNodeName = boneNode.first;
			lowestDistance = distance;
		}
	}

	{
		// Build hierachy such that a child node never appears before a parent node:
		auto& boneHierarchy = skeleton.BoneHierarchy;
		auto& bones = skeleton.Bones;
		std::function<void(aiNode*)> appendChildrenToHierarchy = [&bones, &boneNames, &boneHierarchy, &boneNodes, &appendChildrenToHierarchy](aiNode* node)
		{
			std::string nodeName(node->mName.C_Str());
			if (boneNames.find(nodeName) != boneNames.end())
			{
				bones.emplace_back(nodeName);

				const auto& boneNode = boneNodes.at(nodeName);
				auto parentLocation = std::find(bones.begin(), bones.end(), boneNode->mParent->mName.C_Str());

				// If root bone:
				if (parentLocation == bones.end())
				{
					boneHierarchy.emplace_back(-1);
				}
				// If not root bone:
				else
				{
					auto indexOfParent = std::distance(bones.begin(), parentLocation);
					boneHierarchy.emplace_back(static_cast<EigenVertex::BoneIndexType>(indexOfParent));
				}
			}

			for (std::size_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex)
				appendChildrenToHierarchy(node->mChildren[childIndex]);
		};
		appendChildrenToHierarchy(boneNodes.at(rootNodeName));
	}

	// Insert bone offsets in the correct order:
	skeleton.BoneOffsets.reserve(skeleton.Bones.size());
	for(const auto& boneName : skeleton.Bones)
		skeleton.BoneOffsets.emplace_back(boneOffsets.at(boneName));

	return skeleton;
}

void SceneImporter::AddBoneData(const Skeleton& skeleton, const aiMesh& mesh, Geometry& geometry)
{
	auto& vertices = geometry.MeshData.Vertices;

	for (std::size_t boneIndex = 0; boneIndex < mesh.mNumBones; ++boneIndex)
	{
		auto bone = mesh.mBones[boneIndex];

		auto boneLocation = std::find(skeleton.Bones.begin(), skeleton.Bones.end(), bone->mName.C_Str());
		auto indexOf = std::distance(skeleton.Bones.begin(), boneLocation);

		for (std::size_t vertexWeightIndex = 0; vertexWeightIndex < bone->mNumWeights; ++vertexWeightIndex)
		{
			const auto& vertexWeight = bone->mWeights[vertexWeightIndex];

			auto& vertex = vertices[vertexWeight.mVertexId];
			vertex.BoneWeights.push_back(vertexWeight.mWeight);
			vertex.BoneIndices.push_back(static_cast<EigenVertex::BoneIndexType>(indexOf));
		}
	}
}
