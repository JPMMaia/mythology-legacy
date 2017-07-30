#pragma once
#include <DirectXMath.h>

namespace DirectX12Engine
{
	namespace ShaderBufferTypes
	{
		struct InstanceData
		{
			DirectX::XMFLOAT4X4 ModelMatrix;
			uint32_t MaterialIndex;
			uint32_t Pad0;
			uint32_t Pad1;
			uint32_t Pad2;
		};

		struct MaterialData
		{
			DirectX::XMFLOAT4 BaseColor;
		};

		struct PassData
		{
			DirectX::XMFLOAT4X4 ViewMatrix;
			DirectX::XMFLOAT4X4 ProjectionMatrix;
			DirectX::XMFLOAT4X4 ViewProjectionMatrix;
			DirectX::XMFLOAT4X4 InverseViewProjectionMatrix;
			DirectX::XMFLOAT3 CameraPositionW;
			float Padding;
		};
	}
}
