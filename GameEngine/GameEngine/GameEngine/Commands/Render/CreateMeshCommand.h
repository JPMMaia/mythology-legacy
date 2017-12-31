#pragma once

#include "../ICommand.h"
#include "RenderEngine/IScene.h"
#include "GameEngine/Component/Meshes/MeshComponent.h"

namespace GameEngine
{
	class CreateMeshCommand : public ICommand
	{
	public:
		CreateMeshCommand(RenderEngine::IScene& scene, const std::shared_ptr<BaseMeshComponent>& mesh) :
			m_scene(scene),
			m_mesh(mesh)
		{
		}

	public:
		void Execute() override
		{
			m_scene.CreateMesh(m_mesh->GetName(), m_mesh->GenerateMeshData());
		}

	private:
		RenderEngine::IScene& m_scene;
		std::shared_ptr<BaseMeshComponent> m_mesh;
	};
}