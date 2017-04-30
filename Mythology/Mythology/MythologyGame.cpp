#include "MythologyGame.h"

using namespace Mythology;
using namespace GameEngine;

void MythologyGame::Initialize()
{
	m_gameManager = std::make_shared<GameEngine::GameManager>();
	m_gameManager->Initialize();

	m_person.AddComponent("Box", m_gameManager->CreateBox(BoxGeometry(1.0f, 1.0f, 1.0f, 0)));
}

void MythologyGame::FixedUpdate(const Common::Timer& timer) const
{
	m_gameManager->FixedUpdate(timer);
}
void MythologyGame::FrameUpdate(const Common::Timer& timer) const
{
	m_gameManager->FrameUpdate(timer);
}

std::weak_ptr<GameManager> MythologyGame::GetGameManager() const
{
	return m_gameManager;
}
