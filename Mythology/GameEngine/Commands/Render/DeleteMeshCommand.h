#pragma once

#include "../ICommand.h"
#include "Interfaces/IRenderScene.h"

namespace GameEngine
{
	class DeleteMeshCommand : public ICommand
	{
	public:
		DeleteMeshCommand(IRenderScene& scene, const std::shared_ptr<BaseMeshComponent>& mesh) :
			m_scene(scene),
			m_mesh(mesh)
		{
		}

	public:
		void Execute() override
		{
			m_scene.DeleteMesh(m_mesh);
		}

	private:
		IRenderScene& m_scene;
		std::shared_ptr<BaseMeshComponent> m_mesh;
	};
}
