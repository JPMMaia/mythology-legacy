#pragma once

#include "../ICommand.h"
#include "Interfaces/IRenderScene.h"

namespace GameEngine
{
	class CreateMeshCommand : public ICommand
	{
	public:
		CreateMeshCommand(IRenderScene& scene, const std::shared_ptr<BaseMeshComponent>& mesh) :
			m_scene(scene),
			m_mesh(mesh)
		{
		}

	public:
		void Execute() override
		{
			m_scene.CreateMesh(m_mesh);
		}

	private:
		IRenderScene& m_scene;
		std::shared_ptr<BaseMeshComponent> m_mesh;
	};
}