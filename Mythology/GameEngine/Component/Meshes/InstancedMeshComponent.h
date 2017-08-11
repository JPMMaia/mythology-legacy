#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"

namespace GameEngine
{
	class InstancedMeshComponent : public BaseComponent
	{
	public:
		InstancedMeshComponent() = default;
		
		explicit InstancedMeshComponent(std::size_t materialIndex) : 
			m_materialIndex(materialIndex)
		{
		}

	public:
		std::size_t GetMaterialIndex() const { return m_materialIndex; }
		void SetMaterialIndex(std::size_t materialIndex) { m_materialIndex = materialIndex; };

	private:
		std::size_t m_materialIndex = 0;
	};
}
