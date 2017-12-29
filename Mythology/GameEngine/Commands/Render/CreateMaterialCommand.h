#pragma once

#include "../ICommand.h"
#include "Interfaces/IRenderScene.h"

namespace GameEngine
{
	class CreateMaterialCommand : public ICommand
	{
	public:
		CreateMaterialCommand(IRenderScene& scene, const std::shared_ptr<StandardMaterial>& material) :
			m_scene(scene),
			m_material(material)
		{
		}

	public:
		void Execute() override
		{
			m_scene.CreateMaterial(m_material);
		}

	private:
		IRenderScene& m_scene;
		std::shared_ptr<StandardMaterial> m_material;
	};
}
