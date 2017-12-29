#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"
#include "GameEngine/Component/Meshes/InstancedMeshComponent.h"
#include "GameEngine/Memory/StandardAllocator.h"
#include "GameEngine/Geometry/EigenGeometry.h"
#include "GameEngine/Commands/InstanceEventsQueue.h"

namespace GameEngine
{
	class RenderCommandList;

	class BaseMeshComponent
	{
	public:
		explicit BaseMeshComponent(const std::string& name);

	protected:
		virtual ~BaseMeshComponent();

	public:
		virtual EigenMeshData GenerateMeshData() const = 0;

	public:
		template<typename... ArgumentsTypes>
		std::shared_ptr<InstancedMeshComponent> CreateInstance(RenderCommandList& renderCommandList, ArgumentsTypes&& ...arguments)
		{
			auto pointer = m_instances.allocate(sizeof(InstancedMeshComponent));
			new (reinterpret_cast<void*>(pointer)) InstancedMeshComponent(std::forward<ArgumentsTypes>(arguments)...);

			return CreateInstance(renderCommandList, pointer);
		}

	private:
		std::shared_ptr<InstancedMeshComponent> CreateInstance(RenderCommandList& renderCommandList, InstancedMeshComponent* instancePointer);

	public:
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

	public:
		EigenMeshData GenerateMeshData() const override
		{
			return m_geometry.GenerateMeshData<EigenMeshData>();
		}

	private:
		GeometryType m_geometry;
	};
}
