#pragma once

#include "DirectX12Engine/RenderItem/StandardRenderItem.h"

namespace DirectX12Engine
{
	class RenderRectangle
	{
	public:
		static StandardRenderItem Create(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* commandList, Microsoft::WRL::ComPtr<ID3D12Resource>& vertexUploadBuffer, Microsoft::WRL::ComPtr<ID3D12Resource>& indexUploadBuffer);
	};
}
