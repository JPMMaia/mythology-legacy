#pragma once

namespace GameEngine
{
	template<typename TwoDType, typename ThreeDType>
	struct Vertex
	{
		Vertex() = default;
		Vertex(const ThreeDType& position, const ThreeDType& normal, const ThreeDType& tangent, const TwoDType& textureCoordinates) :
			Position(position),
			Normal(normal),
			Tangent(tangent),
			TextureCoordinates(textureCoordinates)
		{
		}

		ThreeDType Position;
		ThreeDType Normal;
		ThreeDType Tangent;
		TwoDType TextureCoordinates;
	};
}
