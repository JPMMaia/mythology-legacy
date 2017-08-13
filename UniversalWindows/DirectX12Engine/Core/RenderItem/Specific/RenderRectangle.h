#pragma once
#include "Core/RenderItem/StandardRenderItem.h"

namespace DirectX12Engine
{
	class RenderRectangle
	{
	public:
		static StandardRenderItem Create(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList);
	};
}
