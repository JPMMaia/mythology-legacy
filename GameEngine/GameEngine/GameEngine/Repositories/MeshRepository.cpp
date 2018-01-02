#include "pch.h"
#include "MeshRepository.h"

using namespace GameEngine;

void MeshRepository::Add(RenderCommandList& renderCommandList, const std::string & name, const std::shared_ptr<BaseMeshComponent>& mesh)
{
	m_meshes.emplace(mesh->GetName(), mesh);
	renderCommandList.CreateMesh(mesh);
}

const std::shared_ptr<GameEngine::BaseMeshComponent>& GameEngine::MeshRepository::Get(const std::string & name)
{
	return m_meshes.at(name);
}

void GameEngine::MeshRepository::Delete(RenderCommandList& renderCommandList, const std::string& name)
{
	auto meshIt = m_meshes.find(name);
	renderCommandList.DeleteMesh(meshIt->second);
	m_meshes.erase(meshIt);
}
