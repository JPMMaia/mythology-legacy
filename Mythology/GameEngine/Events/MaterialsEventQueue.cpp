#include "pch.h"
#include "MaterialsEventQueue.h"

using namespace GameEngine;

Common::Event<MaterialsEventQueue, std::string, const StandardMaterial&> MaterialsEventQueue::OnCreate;
Common::Event<MaterialsEventQueue, std::string, const StandardMaterial&> MaterialsEventQueue::OnUpdate;
Common::Event<MaterialsEventQueue, std::string, const StandardMaterial&> MaterialsEventQueue::OnDelete;

std::queue<MaterialsEventQueue::Pointer<StandardMaterial>> MaterialsEventQueue::s_createQueue;
std::queue<MaterialsEventQueue::Pointer<StandardMaterial>> MaterialsEventQueue::s_updateQueue;
std::queue<MaterialsEventQueue::Pointer<StandardMaterial>> MaterialsEventQueue::s_deleteQueue;

void GameEngine::MaterialsEventQueue::Flush()
{
	while (!s_createQueue.empty())
	{
		OnCreate(*s_createQueue.front());
		s_createQueue.pop();
	}

	while (!s_updateQueue.empty())
	{
		OnUpdate(*s_updateQueue.front());
		s_updateQueue.pop();
	}

	while (!s_deleteQueue.empty())
	{
		OnDelete(*s_deleteQueue.front());
		s_deleteQueue.pop();
	}
}

void GameEngine::MaterialsEventQueue::Create(const Pointer<StandardMaterial>& material)
{
	s_createQueue.push(material);
}

inline void GameEngine::MaterialsEventQueue::Update(const Pointer<StandardMaterial>& material)
{
	s_updateQueue.push(material);
}

inline void GameEngine::MaterialsEventQueue::Delete(const Pointer<StandardMaterial>& material)
{
	s_deleteQueue.push(material);
}
