#pragma once

namespace DirectX12Engine
{
	class MeshGenerator
	{
	public:

		struct Vertex
		{
			Vertex() = default;
			Vertex(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& normal, const DirectX::XMFLOAT3& tangent, const DirectX::XMFLOAT2& textureCoordinates);

			DirectX::XMFLOAT3 Position;
			DirectX::XMFLOAT3 Normal;
			DirectX::XMFLOAT3 Tangent;
			DirectX::XMFLOAT2 TextureCoordinates;
		};

		struct MeshData
		{
			std::vector<Vertex> Vertices;
			std::vector<uint32_t> Indices;

			static MeshData Subdivide(const MeshData& meshData);

		private:
			static Vertex CalculateMidlleVertex(const Vertex& vertex0, const Vertex& vertex1);
		};

	public:
		static MeshData CreateRectangle(float originX, float originY, float width, float height, float depth);
		static MeshData CreateBox(float width, float height, float depth, uint32_t numberOfSubdivisions);

		static void CalculateNormalAndTangentVectors(MeshData& meshData, uint32_t verticesPerFace);
		static void CalculateTangentBitangentNormal(const DirectX::XMFLOAT3& position0, const DirectX::XMFLOAT3& position1, const DirectX::XMFLOAT3& position2, const DirectX::XMFLOAT2& textureCoordinates0, const DirectX::XMFLOAT2 textureCoordidanates1, const DirectX::XMFLOAT2 textureCoordinates2, DirectX::XMFLOAT3& tangent, DirectX::XMFLOAT3& bitangent, DirectX::XMFLOAT3& normal);
	};

}
