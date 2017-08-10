#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"
#include "MeshComponent.h"

namespace GameEngine
{
	template<class GeometryType>
	class InstancedMeshComponent : public BaseComponent
	{
	public:
		explicit InstancedMeshComponent(const std::shared_ptr<MeshComponent<GeometryType>>& meshComponent) :
			m_meshComponent(meshComponent),
			m_instanceIndex(meshComponent->m_instancesData.Push())
		{
		}
		~InstancedMeshComponent()
		{
			m_meshComponent->m_instancesData.Pop(m_instanceIndex);
		}

	public:
		void SetInstanceData(const InstanceData& instanceData) const
		{
			m_meshComponent->m_instancesData[m_instanceIndex] = instanceData;
		}

	private:
		std::shared_ptr<MeshComponent<GeometryType>> m_meshComponent;
		std::size_t m_instanceIndex;
	};
}
