#include "pch.h"
#include "MeshRepository.h"
#include "GameEngine/Events/MeshEventsQueue.h"

using namespace GameEngine;

void MeshRepository::Add(const std::string & name, const std::shared_ptr<BaseMeshComponent>& mesh)
{
	m_meshes.emplace(mesh->GetName(), mesh);
	MeshEventsQueue::Create(mesh);
}

const std::shared_ptr<GameEngine::BaseMeshComponent>& GameEngine::MeshRepository::Get(const std::string & name)
{
	return m_meshes.at(name);
}

void GameEngine::MeshRepository::Delete(const std::string& name)
{
	auto meshIt = m_meshes.find(name);
	MeshEventsQueue::Delete(meshIt->second);
	m_meshes.erase(meshIt);
}
