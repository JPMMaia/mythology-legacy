#include "EigenGeometry.h"

using namespace GameEngine;

EigenMeshData::VertexType EigenMeshData::CalculateMidlleVertex(const VertexType& vertex0, const VertexType& vertex1)
{
	VertexType output;

	output.Position = 0.5f * (vertex0.Position + vertex1.Position);
	output.Normal = (vertex0.Normal + vertex1.Normal).normalized();
	output.Tangent = (vertex0.Tangent + vertex1.Tangent).normalized();
	output.TextureCoordinates = 0.5 * (vertex0.TextureCoordinates + vertex1.TextureCoordinates);

	return output;
}