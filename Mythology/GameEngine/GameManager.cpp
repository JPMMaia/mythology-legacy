#include "GameManager.h"
#include "GameObject/GameObject.h"

#include "GameEngine/Component/Cameras/CameraComponent.h"

using namespace Common;
using namespace GameEngine;

GameManager::GameManager()
{
}

void GameManager::ProcessInput()
{
	m_mouse.Update();
}

template<class T>
void FixedUpdate(const Timer& timer)
{
	std::for_each(T::Allocator::begin(), T::Allocator::end(), [&timer](auto& element)
	{
		element.FixedUpdate(timer);
	});
}
void GameManager::FixedUpdate(const Common::Timer& timer) const
{
	::FixedUpdate<CameraComponent>(timer);
}
void GameManager::FrameUpdate(const Common::Timer& timer) const
{
}
