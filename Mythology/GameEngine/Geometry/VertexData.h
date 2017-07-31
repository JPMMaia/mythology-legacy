#pragma once

namespace GameEngine
{
	template<typename Vector2Type, typename Vector3Type>
	struct Vertex
	{
		Vertex() = default;
		Vertex(const Vector3Type& position, const Vector3Type& normal, const Vector3Type& tangent, const Vector2Type& textureCoordinates) :
			Position(position),
			Normal(normal),
			Tangent(tangent),
			TextureCoordinates(textureCoordinates)
		{
		}

		Vector3Type Position;
		Vector3Type Normal;
		Vector3Type Tangent;
		Vector2Type TextureCoordinates;
	};
}
