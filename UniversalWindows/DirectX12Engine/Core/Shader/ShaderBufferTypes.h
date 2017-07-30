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
			std::uint32_t AlbedoMapIndex;
		};

		struct LightData
		{
			DirectX::XMFLOAT3 Strength = { 0.5f, 0.5f, 0.5f };		// Light color
			float FalloffStart = 1.0f;								// Point/Spot light only
			DirectX::XMFLOAT3 Direction = { 0.0f, -1.0f, 0.0f };	// Directional/Spot light only
			float FalloffEnd = 10.0f;								// Point/Spot light only
			DirectX::XMFLOAT3 Position = { 0.0f, 0.0f, 0.0f };		// Point/Spot light only
			float SpotPower = 64.0f;								// Spot light only
		};

		struct PassData
		{
			DirectX::XMFLOAT4X4 ViewMatrix;
			DirectX::XMFLOAT4X4 ProjectionMatrix;
			DirectX::XMFLOAT4X4 ViewProjectionMatrix;
			DirectX::XMFLOAT4X4 InverseViewProjectionMatrix;
			DirectX::XMFLOAT3 CameraPositionW;
			float Padding;

			static constexpr auto MaxNumLights = 8;
			std::array<LightData, MaxNumLights> Lights;
		};
	}
}
