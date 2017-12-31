#pragma once

#include "../ICommand.h"
#include "RenderEngine/IScene.h"
#include "GameEngine/Component/Meshes/StandardMaterial.h"

namespace GameEngine
{
	class DeleteMaterialCommand : public ICommand
	{
	public:
		DeleteMaterialCommand(RenderEngine::IScene& scene, const std::shared_ptr<StandardMaterial>& material) :
			m_scene(scene),
			m_material(material)
		{
		}

	public:
		void Execute() override
		{
			m_scene.DeleteMaterial(m_material->GetName());
		}

	private:
		RenderEngine::IScene& m_scene;
		std::shared_ptr<StandardMaterial> m_material;
	};
}
