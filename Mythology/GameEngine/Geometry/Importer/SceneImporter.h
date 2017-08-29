#pragma once

#include "GameEngine/Geometry/EigenGeometry.h"
#include "GameEngine/Geometry/Animation/AnimationClip.h"
#include "GameEngine/Geometry/Animation/Armature.h"

#include <deque>
#include <unordered_map>

struct aiScene;
struct aiMaterialProperty;
struct aiAnimation;
struct aiMaterial;
struct aiMesh;
struct aiNode;

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
			std::unordered_map<std::string, std::vector<std::int32_t>> IntegerProperties;
			std::unordered_map<std::string, std::vector<float>> FloatProperties;
			std::unordered_map<std::string, std::vector<double>> DoubleProperties;
			std::unordered_map<std::string, std::string> StringProperties;
			std::string DiffuseTexturePath;
		};
		struct Animation
		{
			std::string Name;
		};
		struct Armature
		{
			std::deque<std::string> Bones;
			std::vector<Eigen::Affine3f> BoneTransforms;
			std::vector<std::int8_t> BoneHierarchy;
			std::unordered_map<std::string, AnimationClip> Animations;
		};
		struct Object
		{
			std::deque<Geometry> Geometries;
			bool IsAnimated = false;
			std::size_t ArmatureIndex;
			Eigen::Affine3f MeshToParentOfBoneRoot;
		};
		struct ImportedScene
		{
			std::deque<Object> Objects;
			std::deque<Material> Materials;
			std::deque<Armature> Armatures;
		};

	public:
		static void Import(const std::wstring& filePath, ImportedScene& importedScene);

	private:
		static void ForEachNode(const aiNode& rootNode, const std::function<void(const aiNode&)>& function);
		static void ForEachMeshOfNode(const aiScene& scene, const aiNode& node, const std::function<void(const aiMesh&)>& function);

		static MeshDataType CreateMeshData(const aiMesh& mesh);
		static Material CreateMaterial(const aiMaterial& material);
		static Armature CreateArmature(const aiScene& scene, const aiNode& meshesNode, Object& object);
		static void AddBoneData(const Armature& armature, const aiMesh& mesh, Geometry& geometry);
		static AnimationClip CreateSkinnedAnimation(const aiAnimation& animationData, const Armature& armature);

		template <typename ContainerType, typename DataType>
		static ContainerType ParseArray(const aiMaterialProperty& property);
	};
}
