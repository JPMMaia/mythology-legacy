#pragma once

#include "../CommandList.h"

#include "CreateMeshCommand.h"
#include "DeleteMeshCommand.h"
#include "CreateMaterialCommand.h"
#include "DeleteMaterialCommand.h"
#include "CreateInstanceCommand.h"

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

		}
		void UpdateInstance(const std::string& name, const Pointer<InstancedMeshComponent>& instance)
		{

		}
		void DeleteInstance(const std::string& name, const Pointer<RenderInfo>& renderInfo)
		{

		}

	private:
		IRenderScene& m_scene;
	};
}
