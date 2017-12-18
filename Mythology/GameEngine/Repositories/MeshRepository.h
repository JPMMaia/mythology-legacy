#pragma once

#include "GameEngine/Component/Meshes/MeshComponent.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace GameEngine
{
	class MeshRepository
	{
	public:
		MeshRepository() = default;
		MeshRepository(const MeshRepository&) = delete;
		MeshRepository& operator=(const MeshRepository&) = delete;
		MeshRepository(MeshRepository&&) = default;
		MeshRepository& operator=(MeshRepository&&) = default;

	public:
		void Add(const std::string& name, const std::shared_ptr<GameEngine::BaseMeshComponent>& mesh);
		const std::shared_ptr<GameEngine::BaseMeshComponent>& Get(const std::string& name);
		void Delete(const std::string& name);

	private:
		std::unordered_map<std::string, std::shared_ptr<GameEngine::BaseMeshComponent>> m_meshes;
	};
}