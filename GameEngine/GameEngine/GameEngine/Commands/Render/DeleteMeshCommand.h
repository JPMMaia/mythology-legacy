#pragma once

#include "../ICommand.h"
#include "RenderEngine/IScene.h"
#include "GameEngine/Component/Meshes/MeshComponent.h"

namespace GameEngine
{
	class DeleteMeshCommand : public ICommand
	{
	public:
		DeleteMeshCommand(RenderEngine::IScene& scene, const std::shared_ptr<BaseMeshComponent>& mesh) :
			m_scene(scene),
			m_mesh(mesh)
		{
		}

	public:
		void Execute() override
		{
			m_scene.DeleteMesh(m_mesh->GetName());
		}

	private:
		RenderEngine::IScene& m_scene;
		std::shared_ptr<BaseMeshComponent> m_mesh;
	};
}
