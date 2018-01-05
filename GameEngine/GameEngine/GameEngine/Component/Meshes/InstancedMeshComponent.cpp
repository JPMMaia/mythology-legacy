#include "pch.h"
#include "InstancedMeshComponent.h"

using namespace GameEngine;

InstancedMeshComponent::InstancedMeshComponent(const std::string& meshName, const std::shared_ptr<StandardMaterial>& material) :
	m_meshName(meshName),
	m_instance(std::make_shared<RenderEngine::Instance>()),
	m_material(material)
{
	m_instance->Material = &m_material->GetData();
}

const std::shared_ptr<StandardMaterial>& InstancedMeshComponent::GetMaterial() const 
{ 
	return m_material; 
}
void InstancedMeshComponent::SetMaterial(const std::shared_ptr<StandardMaterial>& material) 
{ 
	m_material = material; 
	m_instance->Material = &m_material->GetData();
}

const std::shared_ptr<RenderEngine::Instance>& InstancedMeshComponent::GetInstanceData() const
{
	return m_instance;
}

const std::string& GameEngine::InstancedMeshComponent::GetMeshName() const
{
	return m_meshName;
}
