#pragma once

#include <list>

#include "GameEngine/GameManager.h"
#include "RenderItems/ColorRenderItem.h"

namespace OpenGLESRenderer
{
	class Renderer
	{
	public:
		explicit Renderer(const std::shared_ptr<GameEngine::GameManager>& gameManager);
		~Renderer();

		void Initialize(float width, float height);
		void Shutdown();

		void FrameUpdate(const Common::Timer& timer);
		void Render(const Common::Timer& timer) const;

	private:
		void OnBoxCreated(void* sender, const GameEngine::MeshComponent<GameEngine::BoxGeometry>& box);

	private:
		std::shared_ptr<GameEngine::GameManager> m_gameManager;

		std::list<ColorRenderItem> m_colorRenderItems;
	};
}
