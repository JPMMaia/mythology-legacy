#include "pch.h"
#include "MeshComponent.h"
#include "GameEngine/Commands/Render/RenderCommandList.h"

using namespace GameEngine;

BaseMeshComponent::BaseMeshComponent(const std::string & name) :
	m_name(name)
{
}
BaseMeshComponent::~BaseMeshComponent()
{
}

std::shared_ptr<InstancedMeshComponent> BaseMeshComponent::CreateInstance(RenderCommandList& renderCommandList, InstancedMeshComponent* instancePointer)
{
	auto deleter = [this, &renderCommandList](void* instancePointer)
	{
		auto instance = reinterpret_cast<InstancedMeshComponent*>(instancePointer);
		renderCommandList.DeleteInstance(m_name, instance->GetInstanceData());
		m_instances.deallocate(instance, sizeof(InstancedMeshComponent));
	};

	std::shared_ptr<InstancedMeshComponent> instance(instancePointer, deleter);
	renderCommandList.CreateInstance(m_name, instance);

	return instance;
}
