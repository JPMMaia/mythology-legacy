#pragma once

#include "GameEngine/Component/Meshes/MeshComponent.h"
#include "GameEngine/Component/Meshes/StandardMaterial.h"
#include "GameEngine/Component/Meshes/InstancedMeshComponent.h"

#include <memory>

namespace GameEngine
{
	class IRenderScene
	{
	public:
		virtual void CreateMesh(const std::shared_ptr<BaseMeshComponent>& mesh) = 0;
		virtual void DeleteMesh(const std::shared_ptr<BaseMeshComponent>& mesh) = 0;

	public:
		virtual void CreateMaterial(const std::shared_ptr<StandardMaterial>& material) = 0;
		virtual void DeleteMaterial(const std::shared_ptr<StandardMaterial>& material) = 0;

	public:
		virtual void CreateInstance(const std::string& meshName, const std::shared_ptr<InstancedMeshComponent>& instance) = 0;
		virtual void UpdateInstance(const std::string& meshName, const std::shared_ptr<InstancedMeshComponent>& instance) = 0;
		virtual void DeleteInstance(const std::string& meshName, const std::shared_ptr<RenderInfo>& renderInfo) = 0;
	};
}