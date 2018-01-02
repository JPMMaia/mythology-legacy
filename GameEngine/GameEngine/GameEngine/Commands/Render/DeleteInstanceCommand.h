#pragma once

#include "../ICommand.h"
#include "RenderEngine/IScene.h"
#include "GameEngine/Component/Meshes/InstancedMeshComponent.h"

namespace GameEngine
{
	class DeleteInstanceCommand : public ICommand
	{
	public:
		DeleteInstanceCommand(RenderEngine::IScene& scene, const std::string& meshName, const std::shared_ptr<RenderEngine::Instance>& instanceData) :
			m_scene(scene),
			m_meshName(meshName),
			m_instanceData(instanceData)
		{
		}

	public:
		void Execute() override
		{
			m_scene.DeleteInstance(m_meshName, *m_instanceData);
		}

	private:
		RenderEngine::IScene& m_scene;
		std::string m_meshName;
		std::shared_ptr<RenderEngine::Instance> m_instanceData;
	};
}
