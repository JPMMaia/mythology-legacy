#pragma once

#include "../ICommand.h"
#include "Interfaces/IRenderScene.h"

namespace GameEngine
{
	class DeleteInstanceCommand : public ICommand
	{
	public:
		DeleteInstanceCommand(IRenderScene& scene, const std::string& meshName, const std::shared_ptr<RenderInfo>& renderInfo) :
			m_scene(scene),
			m_meshName(meshName),
			m_renderInfo(renderInfo)
		{
		}

	public:
		void Execute() override
		{
			m_scene.DeleteInstance(m_meshName, m_renderInfo);
		}

	private:
		IRenderScene& m_scene;
		std::string m_meshName;
		std::shared_ptr<RenderInfo> m_renderInfo;
	};
}
