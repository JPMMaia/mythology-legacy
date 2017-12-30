#pragma once

#include "Geometry/MeshData.h"
#include "Materials/Material.h"
#include "Instances/Instance.h"

namespace Common
{
	class Timer;
}

namespace RenderEngine
{
	class IScene
	{
	public:
		virtual void CreateDeviceDependentResources() = 0;
		virtual void CreateWindowSizeDependentResources() = 0;
		virtual bool FrameUpdate(const Common::Timer& timer) = 0;
		virtual bool Render(const Common::Timer& timer) = 0;

	public:
		virtual void CreateMesh(const std::string& name, const MeshData& meshData) = 0;
		virtual void DeleteMesh(const std::string& name) = 0;
		
		virtual void CreateMaterial(const std::string& name, const Material& material) = 0;
		virtual void UpdateMaterial(const std::string& name, const Material& material) = 0;
		virtual void DeleteMaterial(const std::string& name) = 0;

		virtual void CreateInstance(const std::string& meshName, const Instance& instance) = 0;
		virtual void UpdateInstance(const std::string& meshName, const Instance& instance) = 0;
		virtual void DeleteInstance(const std::string& meshName, const std::size_t index) = 0;
	};
}
