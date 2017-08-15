#pragma once

namespace GameEngine
{
	template<typename MeshDataType>
	class CustomGeometry
	{
	public:
		CustomGeometry() = default;
		explicit CustomGeometry(const MeshDataType& meshData) :
			m_meshData(meshData)
		{
		}
		explicit CustomGeometry(MeshDataType&& meshData) :
			m_meshData(std::move(meshData))
		{
		}

	public:
		const MeshDataType& GenerateMeshData() const
		{
			return m_meshData;
		}

	private:
		MeshDataType m_meshData;
	};
}
