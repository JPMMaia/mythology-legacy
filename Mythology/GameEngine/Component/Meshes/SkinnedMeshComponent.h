#pragma once

#include "MeshComponent.h"
#include "GameEngine/Geometry/Primitives/CustomGeometry.h"
#include "GameEngine/Geometry/EigenGeometry.h"
#include "GameEngine/Component/Animations/SkinnedAnimationComponent.h"
#include "GameEngine/Memory/UseStorage.h"

#include <functional>

namespace GameEngine
{
	class SkinnedMeshInstance : public BaseComponent
	{
	public:
		template<typename... ArgumentsTypes>
		explicit SkinnedMeshInstance(ArgumentsTypes&& ...arguments) :
			m_animation(std::forward<ArgumentsTypes>(arguments)...)
		{
		}

	public:
		const SkinnedAnimationComponent& GetAnimation() const
		{
			return m_animation;
		}

	private:
		SkinnedAnimationComponent m_animation;
	};

	class SkinnedMeshComponent : public UseStorage<SkinnedMeshComponent>
	{
	public:
		explicit SkinnedMeshComponent(const std::string& name);

	public:

		void AddMesh(CustomGeometry<EigenMeshData>&& geometry, const std::shared_ptr<StandardMaterial>& material)
		{
			m_geometries.emplace_back(std::move(geometry));
			m_materials.emplace_back(material);
		}

		const std::vector<CustomGeometry<EigenMeshData>>& GetGeometries() const;
		const std::vector<std::shared_ptr<StandardMaterial>>& GetMaterials() const;

	public:

		template<typename... ArgumentsTypes>
		std::shared_ptr<SkinnedMeshInstance> CreateInstance(ArgumentsTypes&& ...arguments)
		{
			using ComponentType = SkinnedMeshInstance;

			auto pointer = m_instances.allocate(sizeof(ComponentType));

			auto deleter = [this](void* pointer)
			{
				m_instances.deallocate(reinterpret_cast<ComponentType*>(pointer), sizeof(ComponentType));
			};
			new (reinterpret_cast<void*>(pointer)) ComponentType(std::forward<ArgumentsTypes>(arguments)...);

			return std::shared_ptr<ComponentType>(pointer, deleter);
		}
		
		StandardAllocator<SkinnedMeshInstance>::iterator GetInstancesBegin()
		{
			return m_instances.begin();
		}
		StandardAllocator<SkinnedMeshInstance>::iterator GetInstancesEnd()
		{
			return m_instances.end();
		}
		std::size_t GetInstancesCount() const
		{
			return m_instances.size();
		}

	public:
		const std::string& GetName() const
		{
			return m_name;
		}

	private:
		std::string m_name;
		std::vector<CustomGeometry<EigenMeshData>> m_geometries;
		std::vector<std::shared_ptr<StandardMaterial>> m_materials;
		StandardAllocator<SkinnedMeshInstance> m_instances;
	};
}
