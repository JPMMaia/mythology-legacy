#pragma once

#include "RenderEngine/Instances/Instance.h"
#include "GameEngine/Component/IComponent.h"
#include "StandardMaterial.h"

#include <memory>

namespace GameEngine
{
	class InstancedMeshComponent : public IComponent
	{
	public:
		explicit InstancedMeshComponent(const std::shared_ptr<StandardMaterial>& material);

	public:
		const std::shared_ptr<StandardMaterial>& GetMaterial() const;
		void SetMaterial(const std::shared_ptr<StandardMaterial>& material);;

		const std::shared_ptr<RenderEngine::Instance>& GetInstanceData() const;

	private:
		std::shared_ptr<RenderEngine::Instance> m_instance;
		std::shared_ptr<StandardMaterial> m_material;
	};
}
