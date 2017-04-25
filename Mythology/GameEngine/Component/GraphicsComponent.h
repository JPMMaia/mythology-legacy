#pragma once

#include "IComponent.h"
#include "TransformComponent.h"
#include "GraphicsEngineInterfaces/IRenderItem.h"
#include "GraphicsEngineInterfaces/IMaterial.h"

#include <memory>

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
