#pragma once

#include "../ICommand.h"
#include "RenderEngine/IScene.h"
#include "GameEngine/Component/Meshes/InstancedMeshComponent.h"

namespace GameEngine
{
	class CreateInstanceCommand : public ICommand
	{
	public:
		CreateInstanceCommand(RenderEngine::IScene& scene, const std::string& meshName, const std::shared_ptr<InstancedMeshComponent>& instance) :
			m_scene(scene),
			m_meshName(meshName),
			m_instance(instance)
		{
		}

	public:
		void Execute() override
		{
			m_scene.CreateInstance(m_meshName, m_instance->GetInstanceData());
		}

	private:
		RenderEngine::IScene& m_scene;
		std::string m_meshName;
		std::shared_ptr<InstancedMeshComponent> m_instance;
	};
}
