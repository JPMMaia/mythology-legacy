#include "pch.h"
#include "InstancedMeshComponent.h"
#include "GameEngine/Component/Meshes/MeshComponent.h"
#include "GameEngine/Commands/InstanceEventsQueue.h"

using namespace GameEngine;

InstancedMeshComponent::InstancedMeshComponent(const std::shared_ptr<StandardMaterial>& material) :
	m_renderInfo(std::make_shared<RenderInfo>()),
	m_material(material)
{
}

const std::shared_ptr<StandardMaterial>& GameEngine::InstancedMeshComponent::GetMaterial() const { return m_material; }
void GameEngine::InstancedMeshComponent::SetMaterial(const std::shared_ptr<StandardMaterial>& material) { m_material = material; }

const std::shared_ptr<RenderInfo>& GameEngine::InstancedMeshComponent::GetRenderInfo() const { return m_renderInfo; }
