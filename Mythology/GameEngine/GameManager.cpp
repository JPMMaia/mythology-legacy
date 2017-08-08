#include "GameManager.h"
#include "GameObject/GameObject.h"

using namespace Common;
using namespace GameEngine;

GameManager::GameManager()
{
}

template<class T>
void FixedUpdate(const Timer& timer)
{
	for(auto it = T::Allocator::begin(); it != T::Allocator::end(); ++it)
	{
		it->FixedUpdate(timer);
	}

	/*std::for_each(T::Allocator::begin(), T::Allocator::end(), [&timer](auto& element)
	{
		element.FixedUpdate(timer);
	});*/
}
void GameManager::FixedUpdate(const Common::Timer& timer) const
{
	::FixedUpdate<CameraComponent>(timer);
}
void GameManager::FrameUpdate(const Common::Timer& timer) const
{
}
