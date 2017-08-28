#include "SceneImporter.h"
#include "Common/Helpers.h"
#include "GameEngine/Geometry/Animation/AnimationClip.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <iterator>
#include <unordered_set>
#include <queue>

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

	// Create geometries of each object:
	auto& objects = importedScene.Objects;
	std::unordered_map<const aiNode*, Object*> objectNodes;
	ForEachNode(*scene->mRootNode, [&scene, &objects, &objectNodes](const aiNode& node)
	{
		if (node.mNumMeshes == 0)
			return;

		Object object;
		for (std::size_t meshIndex = 0; meshIndex < node.mNumMeshes; ++meshIndex)
		{
			const auto& mesh = *scene->mMeshes[node.mMeshes[meshIndex]];

			Geometry geometry;
			geometry.Name = mesh.mName.C_Str();
			geometry.MeshData = CreateMeshData(mesh);
			geometry.MaterialIndex = mesh.mMaterialIndex;
			object.Geometries.emplace_back(std::move(geometry));
		}
		
		objects.emplace_back(std::move(object));
		objectNodes.emplace(&node, &objects.back());
	});

	// Create armature of each object:
	for (const auto& objectNodePair : objectNodes)
	{
		const auto* node = objectNodePair.first;
		auto* object = objectNodePair.second;

		object->IsAnimated = false;		
		for (std::size_t meshIndex = 0; meshIndex < node->mNumMeshes; ++meshIndex)
		{
			auto mesh = scene->mMeshes[node->mMeshes[meshIndex]];
			if (mesh->HasBones())
			{
				object->IsAnimated = true;
				break;
			}
		}

		if (object->IsAnimated)
		{
			// Create armature:
			importedScene.Armatures.emplace_back(CreateArmature(*scene, *node, *object));
			auto& armature = importedScene.Armatures.back();
			object->ArmatureIndex = importedScene.Armatures.size() - 1;

			// Add bone weights and indices to the mesh data:
			for (std::size_t meshIndex = 0; meshIndex < node->mNumMeshes; ++meshIndex)
			{
				auto& mesh = *scene->mMeshes[node->mMeshes[meshIndex]];
				auto& geometry = object->Geometries[meshIndex];
				geometry.MeshData.ContainsSkinnedData = true;
				AddBoneData(armature, mesh, geometry);
			}

			// Create animation clips:
			auto& animations = armature.Animations;
			for (std::size_t i = 0; i < scene->mNumAnimations; ++i)
			{
				const auto& animationData = *scene->mAnimations[i];
				animations.emplace(animationData.mName.C_Str(), CreateSkinnedAnimation(animationData, armature));
			}
		}
	}

	for (std::size_t i = 0; i < scene->mNumMaterials; ++i)
	{
		importedScene.Materials.emplace_back(CreateMaterial(*scene->mMaterials[i]));
	}
}

void SceneImporter::ForEachNode(const aiNode& rootNode, const std::function<void(const aiNode&)>& function)
{
	std::function<void(const aiNode&)> applyAndIterateThroughChildren = [&function, &applyAndIterateThroughChildren](const aiNode& node)
	{
		function(node);

		for (std::size_t childIndex = 0; childIndex < node.mNumChildren; ++childIndex)
			applyAndIterateThroughChildren(*node.mChildren[childIndex]);
	};
	applyAndIterateThroughChildren(rootNode);
}
void SceneImporter::ForEachMeshOfNode(const aiScene& scene, const aiNode& node, const std::function<void(const aiMesh&)>& function)
{
	for (std::size_t meshIndex = 0; meshIndex < node.mNumMeshes; ++meshIndex)
		function(*scene.mMeshes[node.mMeshes[meshIndex]]);
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
SceneImporter::Armature SceneImporter::CreateArmature(const aiScene& scene, const aiNode& meshesNode, Object& object)
{
	Armature armature;

	// Find all bones:
	std::unordered_set<aiNode*> neededBones;
	{
		for (std::size_t meshIndex = 0; meshIndex < meshesNode.mNumMeshes; meshIndex++)
		{
			auto mesh = scene.mMeshes[meshesNode.mMeshes[meshIndex]];
			for (std::size_t boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
			{
				auto bone = mesh->mBones[boneIndex];

				// Mark bone and all its parents as needed, until the mesh node or the parent of the mesh node is found:
				for (auto boneNode = scene.mRootNode->FindNode(bone->mName); boneNode != nullptr; boneNode = boneNode->mParent)
				{
					// Stop if mesh node or parent of the mesh node:
					if (boneNode == &meshesNode || boneNode == meshesNode.mParent)
						break;

					// Mark bone as needed:
					neededBones.emplace(boneNode);
				}
			}
		}

		// Ensure that all bone children are in the list:
		for (auto boneNode : neededBones)
			for (std::size_t childIndex = 0; childIndex < boneNode->mNumChildren; ++childIndex)
				neededBones.emplace(boneNode->mChildren[childIndex]);
	}

	// Find the root node of the skeleton:
	auto lowestDistance = (std::numeric_limits<std::size_t>::max)();
	aiNode* rootNode = nullptr;
	{
		for (const auto& boneNode : neededBones)
		{
			std::size_t distance = 0;

			for (auto currentNode = boneNode->mParent; currentNode != nullptr; currentNode = currentNode->mParent)
				++distance;

			if (distance < lowestDistance)
			{
				rootNode = boneNode;
				lowestDistance = distance;
			}
		}
	}

	{
		// Build hierachy such that a child node never appears before a parent node:
		auto& boneHierarchy = armature.BoneHierarchy;
		auto& bones = armature.Bones;
		auto& transforms = armature.BoneTransforms;
		std::function<void(aiNode*)> appendChildrenToHierarchy = [&bones, &boneHierarchy, &transforms, &scene, &appendChildrenToHierarchy](aiNode* node)
		{
			std::string nodeName(node->mName.C_Str());
			bones.emplace_back(nodeName);

			{
				// Find the parent of the node:
				auto parentLocation = std::find(bones.begin(), bones.end(), node->mParent->mName.C_Str());

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

			{
				Eigen::Matrix4f transform;
				{
					const auto& m = node->mTransformation;
					transform <<
						m.a1, m.a2, m.a3, m.a4,
						m.b1, m.b2, m.b3, m.b4,
						m.c1, m.c2, m.c3, m.c4,
						m.d1, m.d2, m.d3, m.d4;
				}
				transforms.emplace_back(Eigen::Affine3f(transform));
			}

			for (std::size_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex)
				appendChildrenToHierarchy(node->mChildren[childIndex]);
		};
		appendChildrenToHierarchy(rootNode);
	}

	// Calculate the mesh to bone root space matrix:
	{
		// MeshToSceneRoot = ROOT * M1 * M2 * ...
		// BoneRootToSceneRoot = ROOT * O1 * O2 * BoneRoot
		// SceneRootToBoneRoot = BoneRoot^(-1) * O2^(-1) * O1^(-1) * ROOT^(-1)
		// MeshToBoneRoot = BoneRoot^(-1) * O2^(-1) * O1^(-1) * M1 * M2 * ...

		auto meshToSceneRoot = Eigen::Affine3f::Identity();
		for (auto currentNode = &meshesNode; currentNode != scene.mRootNode; currentNode = currentNode->mParent)
		{
			Eigen::Matrix4f transform;
			{
				const auto& m = currentNode->mTransformation;
				transform <<
					m.a1, m.a2, m.a3, m.a4,
					m.b1, m.b2, m.b3, m.b4,
					m.c1, m.c2, m.c3, m.c4,
					m.d1, m.d2, m.d3, m.d4;
			}

			meshToSceneRoot = Eigen::Affine3f(transform) * meshToSceneRoot;
		}

		auto sceneRootToBoneRoot = Eigen::Affine3f::Identity();
		for (auto currentNode = rootNode; currentNode != scene.mRootNode; currentNode = currentNode->mParent)
		{
			Eigen::Matrix4f transform;
			{
				const auto& m = currentNode->mTransformation;
				transform <<
					m.a1, m.a2, m.a3, m.a4,
					m.b1, m.b2, m.b3, m.b4,
					m.c1, m.c2, m.c3, m.c4,
					m.d1, m.d2, m.d3, m.d4;
			}
			
			sceneRootToBoneRoot = sceneRootToBoneRoot * Eigen::Affine3f(transform).inverse();
		}

		object.MeshToBoneRoot = sceneRootToBoneRoot * meshToSceneRoot;
	}

	return armature;
}
void SceneImporter::AddBoneData(const Armature& armature, const aiMesh& mesh, Geometry& geometry)
{
	auto& vertices = geometry.MeshData.Vertices;

	for (std::size_t boneIndex = 0; boneIndex < mesh.mNumBones; ++boneIndex)
	{
		auto bone = mesh.mBones[boneIndex];

		auto boneLocation = std::find(armature.Bones.begin(), armature.Bones.end(), bone->mName.C_Str());
		auto indexOf = std::distance(armature.Bones.begin(), boneLocation);

		for (std::size_t vertexWeightIndex = 0; vertexWeightIndex < bone->mNumWeights; ++vertexWeightIndex)
		{
			const auto& vertexWeight = bone->mWeights[vertexWeightIndex];

			auto& vertex = vertices[vertexWeight.mVertexId];
			vertex.BoneWeights.push_back(vertexWeight.mWeight);
			vertex.BoneIndices.push_back(static_cast<EigenVertex::BoneIndexType>(indexOf));
		}
	}
}
AnimationClip SceneImporter::CreateSkinnedAnimation(const aiAnimation& animationData, const Armature& skeleton)
{
	std::vector<BoneAnimation> boneAnimations;
	boneAnimations.reserve(static_cast<std::size_t>(animationData.mNumChannels));
	for (std::size_t channelIndex = 0; channelIndex < animationData.mNumChannels; ++channelIndex)
	{
		auto channel = animationData.mChannels[channelIndex];

		// Do not add animations that do not refer to a bone:
		if (std::find(skeleton.Bones.begin(), skeleton.Bones.end(), channel->mNodeName.C_Str()) == skeleton.Bones.end())
			continue;

		std::vector<Keyframe<Eigen::Vector3f>> positionKeyframes(channel->mNumPositionKeys);
		{
			auto& keyframes = positionKeyframes;
			for (std::size_t keyIndex = 0; keyIndex < keyframes.size(); ++keyIndex)
			{
				const auto& key = channel->mPositionKeys[keyIndex];
				keyframes[keyIndex].TimePosition = static_cast<float>(key.mTime);
				keyframes[keyIndex].Value = { key.mValue.x, key.mValue.y, key.mValue.z };
			}
		}

		std::vector<Keyframe<Eigen::Quaternionf>> rotationKeyframes(channel->mNumRotationKeys);
		{
			auto& keyframes = rotationKeyframes;
			for (std::size_t keyIndex = 0; keyIndex < keyframes.size(); ++keyIndex)
			{
				const auto& key = channel->mRotationKeys[keyIndex];
				keyframes[keyIndex].TimePosition = static_cast<float>(key.mTime);
				keyframes[keyIndex].Value = Eigen::Quaternionf(key.mValue.w, key.mValue.x, key.mValue.y, key.mValue.z);
			}
		}

		std::vector<Keyframe<Eigen::Vector3f>> scaleKeyframes(channel->mNumScalingKeys);
		{
			auto& keyframes = scaleKeyframes;
			for (std::size_t keyIndex = 0; keyIndex < keyframes.size(); ++keyIndex)
			{
				const auto& key = channel->mScalingKeys[keyIndex];
				keyframes[keyIndex].TimePosition = static_cast<float>(key.mTime);
				keyframes[keyIndex].Value = { key.mValue.x, key.mValue.y, key.mValue.z };
			}
		}


		if (!positionKeyframes.empty() || !rotationKeyframes.empty() || !scaleKeyframes.empty())
		{
			boneAnimations.emplace_back(positionKeyframes, rotationKeyframes, scaleKeyframes);
		}
	}

	return AnimationClip(boneAnimations);
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