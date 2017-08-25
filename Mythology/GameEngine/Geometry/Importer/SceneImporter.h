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

			friend std::ostream& operator<<(std::ostream& outputStream, const Geometry& geometry);
			friend std::istream& operator>>(std::istream& inputStream, Geometry& geometry);
		};
		struct Material
		{
			std::unordered_map<std::string, std::vector<std::int32_t>> IntegerProperties;
			std::unordered_map<std::string, std::vector<float>> FloatProperties;
			std::unordered_map<std::string, std::vector<double>> DoubleProperties;
			std::unordered_map<std::string, std::string> StringProperties;
			std::string DiffuseTexturePath;

			template<class KeyType, class ValueType, class MapType>
			friend void InputMap(std::istream& inputStream, MapType& map);
			friend std::ostream& operator<<(std::ostream& outputStream, const Material& material);
			friend std::istream& operator>>(std::istream& inputStream, Material& material);
		};
		struct Animation
		{
			std::string Name;
		};
		struct Skeleton
		{
			std::deque<std::string> Bones;
			std::vector<Eigen::Affine3f> BoneTransforms;
			std::vector<std::int8_t> BoneHierarchy;
		};
		struct Object
		{
			std::queue<Geometry> Geometries;
			Skeleton Skeleton;
			bool IsAnimated;
		};
		struct ImportedScene
		{
			std::deque<Object> Objects;
			std::deque<Material> Materials;
			Armature Armature;

			friend std::ostream& operator<<(std::ostream& outputStream, const ImportedScene& importedScene);
			friend std::istream& operator>>(std::istream& inputStream, ImportedScene& importedScene);
		};

	public:
		static void Import(const std::wstring& filePath, ImportedScene& importedScene);

	private:
		static MeshDataType CreateMeshData(const aiMesh& mesh);
		static Material CreateMaterial(const aiMaterial& material);
		static AnimationClip CreateSkinnedAnimation(const aiAnimation& animationData, const Skeleton& skeleton);
		static Skeleton CreateSkeleton(const aiScene& scene);
		static void AddBoneData(const Skeleton& skeleton, const aiMesh& mesh, Geometry& geometry);

		template <typename ContainerType, typename DataType>
		static ContainerType ParseArray(const aiMaterialProperty& property);
	};
}
