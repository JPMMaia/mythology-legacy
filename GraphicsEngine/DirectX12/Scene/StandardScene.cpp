#include "pch.h"
#include "StandardScene.h"
#include "DirectX12/Geometry/ImmutableMesh.h"
#include "DirectX12/Geometry/MeshGenerator.h"

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
	// Add cube:
	{
		auto meshData = MeshGenerator::CreateBox(1.0f, 1.0f, 1.0f, 0);
		ImmutableMesh mesh("CubeMesh", meshData);
		mesh.AddSubmesh("CubeSubmesh", Submesh(meshData));

		
	}
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
