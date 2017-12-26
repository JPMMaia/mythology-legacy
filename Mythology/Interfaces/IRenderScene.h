#pragma once

#include "GameEngine/Component/Meshes/MeshComponent.h"

#include <memory>

namespace GameEngine
{
	class IRenderScene
	{
	public:
		virtual void CreateMesh(const std::shared_ptr<BaseMeshComponent>& mesh) = 0;
		virtual void DeleteMesh(const std::shared_ptr<BaseMeshComponent>& mesh) = 0;
	};
}