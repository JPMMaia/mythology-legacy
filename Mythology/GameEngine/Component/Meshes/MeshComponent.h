#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"
#include "GameEngine/Component/Meshes/InstancedMeshComponent.h"
#include "GameEngine/Memory/StandardAllocator.h"

namespace GameEngine
{
	class BaseMeshComponent
	{
	public:
		explicit BaseMeshComponent(const std::string& name) :
			m_name(name)
		{
		}

		template<typename... ArgumentsTypes>
		std::shared_ptr<InstancedMeshComponent> CreateInstance(ArgumentsTypes&& ...arguments)
		{
			auto pointer = m_instances.allocate(sizeof(InstancedMeshComponent));

			auto deleter = [this](void* pointer)
			{
				m_instances.deallocate(reinterpret_cast<InstancedMeshComponent*>(pointer), sizeof(InstancedMeshComponent));
			};
			new (reinterpret_cast<void*>(pointer)) InstancedMeshComponent(std::forward<ArgumentsTypes>(arguments)...);

			return std::shared_ptr<InstancedMeshComponent>(pointer, deleter);
		}

		StandardAllocatorIterator<InstancedMeshComponent> InstancesBegin()
		{
			return m_instances.begin();
		}
		StandardAllocatorIterator<InstancedMeshComponent> InstancesEnd()
		{
			return m_instances.end();
		}

		std::size_t GetInstanceCount() const
		{
			return m_instances.size();
		}

		const std::string& GetName() const
		{
			return m_name;
		}

	private:
		std::string m_name;
		StandardAllocator<InstancedMeshComponent> m_instances;
	};

	template<typename GeometryType>
	class MeshComponent : public BaseComponent, public BaseMeshComponent, public UseStorage<MeshComponent<GeometryType>>
	{
	public:
		MeshComponent() = default;
		explicit MeshComponent(const std::string& name, const GeometryType& geometry) :
			BaseMeshComponent(name),
			m_geometry(geometry)
		{
		}

	public:
		const GeometryType& GetGeometry() const
		{
			return m_geometry;
		}
		GeometryType& GetGeometry()
		{
			return m_geometry;
		}

	private:
		GeometryType m_geometry;
	};
}
