#include "pch.h"
#include "StandardScene.h"

using namespace GraphicsEngine;

StandardScene::StandardScene()
{
	StandardScene::CreateDeviceDependentResources();
	StandardScene::CreateWindowSizeDependentResources();

	StandardScene::LoadState();
}
StandardScene::~StandardScene()
{
	StandardScene::SaveState();
}

void StandardScene::CreateDeviceDependentResources()
{
}
void StandardScene::CreateWindowSizeDependentResources()
{
}

void StandardScene::SaveState()
{
}
void StandardScene::LoadState()
{
}

void StandardScene::FrameUpdate(const Common::Timer& timer)
{
}
void StandardScene::FixedUpdate(const Common::Timer& timer)
{
}
