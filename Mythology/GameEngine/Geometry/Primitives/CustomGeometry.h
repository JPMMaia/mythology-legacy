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
		template<class T, typename = std::enable_if_t<std::is_same<T, MeshDataType>::value>>
		const T& GenerateMeshData() const
		{
			return m_meshData;
		}

	private:
		MeshDataType m_meshData;
	};
}
