#pragma once

#include <vector>
#include <cstdint>

namespace GameEngine
{
	template<typename Vector2, typename Vector3>
	struct Vertex
	{
	public:
		using BoneIndexType = std::uint8_t;

	public:
		Vertex() = default;
		Vertex(const Vector3& position, const Vector3& normal, const Vector3& tangent, const Vector2& textureCoordinates) :
			Position(position),
			Normal(normal),
			Tangent(tangent),
			TextureCoordinates(textureCoordinates)
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
