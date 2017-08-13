#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"
#include "StandardMaterial.h"

namespace GameEngine
{
	class InstancedMeshComponent : public BaseComponent
	{
	public:
		InstancedMeshComponent() = default;
		
		explicit InstancedMeshComponent(const std::shared_ptr<StandardMaterial>& material) : 
			m_material(material)
		{
		}

	public:
		const std::shared_ptr<StandardMaterial>& GetMaterial() const { return m_material; }
		void SetMaterial(const std::shared_ptr<StandardMaterial>& material) { m_material = material; };

	private:
		std::shared_ptr<StandardMaterial> m_material;
	};
}
