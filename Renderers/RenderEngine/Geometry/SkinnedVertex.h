#pragma once

#include <Eigen/Eigen>

#include <cstdint>
#include <vector>

namespace RenderEngine
{
	struct SkinnedVertex
	{
	public:
		using Vector2 = Eigen::Vector2f;
		using Vector3 = Eigen::Vector3f;
		using BoneIndexType = std::uint8_t;

	public:
		SkinnedVertex() = default;
		SkinnedVertex(const Vector3& position, const Vector3& normal, const Vector3& tangent, const Vector2& textureCoordinates, const std::vector<float>& boneWeights, const std::vector<BoneIndexType>& boneIndices) :
			Position(position),
			Normal(normal),
			Tangent(tangent),
			TextureCoordinates(textureCoordinates),
			BoneWeights(boneWeights),
			BoneIndices(boneIndices)
		{
		}

		Vector3 Position;
		Vector3 Normal;
		Vector3 Tangent;
		Vector2 TextureCoordinates;
		std::vector<float> BoneWeights;
		std::vector<BoneIndexType> BoneIndices;
	};
}
