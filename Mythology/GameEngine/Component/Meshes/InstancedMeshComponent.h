#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"
#include "StandardMaterial.h"
#include "RenderInfo.h"

#include <memory>

namespace GameEngine
{
	class BaseMeshComponent;

	class InstancedMeshComponent : public BaseComponent
	{
	public:
		explicit InstancedMeshComponent(const std::shared_ptr<StandardMaterial>& material);

	public:
		const std::shared_ptr<StandardMaterial>& GetMaterial() const;
		void SetMaterial(const std::shared_ptr<StandardMaterial>& material);;

		const std::shared_ptr<RenderInfo>& GetRenderInfo() const;

	private:
		std::shared_ptr<RenderInfo> m_renderInfo;
		std::shared_ptr<StandardMaterial> m_material;
	};
}
