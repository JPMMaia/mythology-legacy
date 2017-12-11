#include "pch.h"
#include "MaterialRepository.h"

#include "GameEngine/Events/MaterialEventsQueue.h"

using namespace GameEngine;

void MaterialRepository::Add(const std::string& name, const std::shared_ptr<StandardMaterial>& material)
{
	m_materials.emplace(material->GetName(), material);
	MaterialEventsQueue::Create(material);
}
const std::shared_ptr<StandardMaterial>& MaterialRepository::Get(const std::string& name)
{
	return m_materials.at(name);
}
void MaterialRepository::Delete(const std::string& name)
{
	auto materialIt = m_materials.find(name);
	MaterialEventsQueue::Delete(materialIt->second);
	m_materials.erase(materialIt);
}
