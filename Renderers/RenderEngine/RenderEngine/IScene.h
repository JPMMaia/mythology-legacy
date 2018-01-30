#pragma once

#include "Geometry/MeshData.h"
#include "Geometry/Vertex.h"
#include "Materials/Material.h"
#include "Instances/Instance.h"

namespace Common
{
	class Timer;
}

namespace RenderEngine
{
	struct RenderParameters;

	class IScene
	{
	public:
		virtual ~IScene() = default;

	public:
		virtual void RecreateDeviceDependentResources() = 0;
		virtual void RecreateWindowSizeDependentResources() = 0;

	public:
		virtual void FrameUpdate(const Common::Timer& timer) = 0;
		virtual bool Render(const RenderParameters& renderParameters) = 0;

	public:
		virtual void CreateMesh(const std::string& name, const MeshData<RenderEngine::Vertex, std::uint16_t>& meshData) = 0;
		virtual void CreateMesh(const std::string& name, const MeshData<RenderEngine::Vertex, std::uint32_t>& meshData) = 0;
		virtual void DeleteMesh(const std::string& name) = 0;
		
		virtual void CreateMaterial(const std::string& name, const Material& material) = 0;
		virtual void UpdateMaterial(const std::string& name, const Material& material) = 0;
		virtual void DeleteMaterial(const std::string& name) = 0;

		virtual void CreateInstance(const std::string& meshName, const std::shared_ptr<RenderEngine::Instance>& instance) = 0;
		virtual void UpdateInstance(const std::string& meshName, const RenderEngine::Instance& instance) = 0;
		virtual void DeleteInstance(const std::string& meshName, const RenderEngine::Instance& instance) = 0;
	};
}
