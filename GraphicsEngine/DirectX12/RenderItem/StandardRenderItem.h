#pragma once

#include "Interfaces/IRenderItem.h"

namespace GraphicsEngine
{
	class StandardMaterial;
	class ImmutableMesh;

	class StandardRenderItem : public IRenderItem
	{
	public:
		StandardRenderItem(ImmutableMesh geometry, StandardMaterial material);
		
	};
}
