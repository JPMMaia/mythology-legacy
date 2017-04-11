#pragma once

#include "IComponent.h"
#include "GraphicsEngine/Interfaces/IRenderItem.h"

#include <memory>
#include "TransformComponent.h"
#include "../../GraphicsEngine/Interfaces/IMaterial.h"

namespace GameEngine
{
	class GraphicsComponent : public IComponent
	{
	public:
		GraphicsComponent();
		
	private:
		TransformComponent m_transform;
		std::shared_ptr<GraphicsEngine::IRenderItem> m_renderItem;
		std::shared_ptr<GraphicsEngine::IMaterial> m_material;
	};
}
