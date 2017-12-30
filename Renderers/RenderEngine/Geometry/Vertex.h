#pragma once

#include <Eigen/Eigen>

namespace RenderEngine
{
	struct Vertex
	{
	public:
		using Vector2 = Eigen::Vector2f;
		using Vector3 = Eigen::Vector3f;

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
	};
}
