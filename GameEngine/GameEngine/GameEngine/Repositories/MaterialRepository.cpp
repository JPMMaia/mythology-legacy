#include "pch.h"
#include "MaterialRepository.h"

using namespace GameEngine;

void MaterialRepository::Add(RenderCommandList& renderCommandList, const std::string& name, const std::shared_ptr<StandardMaterial>& material)
{
	m_materials.emplace(material->GetName(), material);
	renderCommandList.CreateMaterial(material);
}
const std::shared_ptr<StandardMaterial>& MaterialRepository::Get(const std::string& name)
{
	return m_materials.at(name);
}
void MaterialRepository::Delete(RenderCommandList& renderCommandList, const std::string& name)
{
	auto materialIt = m_materials.find(name);
	renderCommandList.DeleteMaterial(materialIt->second);
	m_materials.erase(materialIt);
}
