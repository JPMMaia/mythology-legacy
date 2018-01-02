#pragma once

#include "../CommandList.h"

#include "CreateMeshCommand.h"
#include "DeleteMeshCommand.h"
#include "CreateMaterialCommand.h"
#include "DeleteMaterialCommand.h"
#include "CreateInstanceCommand.h"
#include "UpdateInstanceCommand.h"
#include "DeleteInstanceCommand.h"

#include <memory>

namespace GameEngine
{
	class RenderCommandList : public CommandList
	{
	public:
		template<class Type>
		using Pointer = std::shared_ptr<Type>;

	public:
		explicit RenderCommandList(RenderEngine::IScene& scene) : 
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
		void CreateMaterial(const Pointer<StandardMaterial>& material)
		{
			m_commands.emplace_back(std::make_unique<CreateMaterialCommand>(m_scene, material));
		}
		void DeleteMaterial(const Pointer<StandardMaterial>& material)
		{
			m_commands.emplace_back(std::make_unique<DeleteMaterialCommand>(m_scene, material));
		}

	public:
		void CreateInstance(const std::string& name, const Pointer<InstancedMeshComponent>& instance)
		{
			m_commands.emplace_back(std::make_unique<CreateInstanceCommand>(m_scene, name, instance));
		}
		void UpdateInstance(const std::string& name, const Pointer<InstancedMeshComponent>& instance)
		{
			m_commands.emplace_back(std::make_unique<UpdateInstanceCommand>(m_scene, name, instance));
		}
		void DeleteInstance(const std::string& name, const std::shared_ptr<RenderEngine::Instance>& instanceData)
		{
			m_commands.emplace_back(std::make_unique<DeleteInstanceCommand>(m_scene, name, instanceData));
		}

	private:
		RenderEngine::IScene& m_scene;
	};
}
