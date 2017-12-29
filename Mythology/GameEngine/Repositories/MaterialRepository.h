#pragma once

#include "GameEngine/Component/Meshes/StandardMaterial.h"
#include "GameEngine/Commands/Render/RenderCommandList.h"

#include <memory>
#include <string>
#include <unordered_map>

namespace GameEngine
{
	class MaterialRepository
	{
	public:
		MaterialRepository() = default;
		MaterialRepository(const MaterialRepository&) = delete;
		MaterialRepository& operator=(const MaterialRepository&) = delete;
		MaterialRepository(MaterialRepository&&) = default;
		MaterialRepository& operator=(MaterialRepository&&) = default;

	public:
		void Add(RenderCommandList& renderCommandList, const std::string& name, const std::shared_ptr<GameEngine::StandardMaterial>& material);
		const std::shared_ptr<GameEngine::StandardMaterial>& Get(const std::string& name);
		void Delete(RenderCommandList& renderCommandList, const std::string& name);

	private:
		std::unordered_map<std::string, std::shared_ptr<GameEngine::StandardMaterial>> m_materials;
	};
}