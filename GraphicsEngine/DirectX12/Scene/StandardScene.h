#pragma once

#include "Interfaces/IScene.h"
#include "Interfaces/IRenderItem.h"

namespace GraphicsEngine
{
	class StandardScene : public IScene
	{
	public:
		StandardScene();
		~StandardScene();

	public:
		void CreateDeviceDependentResources() override;
		void CreateWindowSizeDependentResources() override;
		
		void SaveState() override;
		void LoadState() override;

		void FrameUpdate(const Common::Timer& timer) override;
		void FixedUpdate(const Common::Timer& timer) override;

	private:
		
	};
}
