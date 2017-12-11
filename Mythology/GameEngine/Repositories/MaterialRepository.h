#pragma once

#include "GameEngine/Component/Meshes/StandardMaterial.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace GameEngine
{
	class MaterialRepository
	{
	public:
		void Add(const std::string& name, const std::shared_ptr<GameEngine::StandardMaterial>& material);
		const std::shared_ptr<GameEngine::StandardMaterial>& Get(const std::string& name);
		void Delete(const std::string& name);

	private:
		std::unordered_map<std::string, std::shared_ptr<GameEngine::StandardMaterial>> m_materials;
	};
}