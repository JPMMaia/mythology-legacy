#pragma once

#include "../CommandList.h"

#include "GameEngine/Component/Meshes/MeshComponent.h"
#include "CreateMeshCommand.h"
#include "DeleteMeshCommand.h"

#include <memory>

namespace GameEngine
{
	class RenderCommandList : public CommandList
	{
	public:
		template<class Type>
		using Pointer = std::shared_ptr<Type>;

	public:
		explicit RenderCommandList(IRenderScene& scene) : 
			m_scene(scene)
		{
		}

	public:
		void CreateMesh(const Pointer<BaseMeshComponent>& mesh)
		{
			m_commands.emplace_back(std::make_unique<CreateMeshCommand>(m_scene, mesh));
		}
		void DeleteMesh(const Pointer<BaseMeshComponent>& mesh)
		{
			m_commands.emplace_back(std::make_unique<DeleteMeshCommand>(m_scene, mesh));
		}

	public:
		void CreateMaterial();
		void DeleteMaterial();

	public:
		void CreateInstance();
		void DeleteInstance();

	private:
		IRenderScene& m_scene;
	};
}