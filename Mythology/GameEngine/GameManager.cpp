#include "GameManager.h"
#include "GameObject/GameObject.h"

using namespace Common;
using namespace GameEngine;

GameManager::GameManager()
{
}

void GameManager::FixedUpdate(const Common::Timer& timer)
{
	auto fixedUpdate = [&timer](auto& container)
	{
		std::for_each(container.Begin(), container.End(), [&timer](auto& element)
		{
			if(element.IsInitialized())
				element.GetElement().FixedUpdate(timer);
		});
	};
	
	//fixedUpdate(CameraComponent::GetStorage());
}
void GameManager::FrameUpdate(const Common::Timer& timer)
{
}
