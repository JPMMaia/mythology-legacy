#pragma once

#include "Core/DeviceResources.h"
#include "ShaderStructures.h"
#include "GraphicsEngineInterfaces/IScene.h"

namespace Common
{
	class Timer;
}

namespace DirectX12Engine
{
	// This sample renderer instantiates a basic rendering pipeline.
	class Sample3DSceneRenderer : IScene
	{
	public:
		explicit Sample3DSceneRenderer(const std::shared_ptr<DirectX12Engine::DeviceResources>& deviceResources);
		~Sample3DSceneRenderer();

	public:
		void CreateDeviceDependentResources() override;
		void CreateWindowSizeDependentResources() override;
		void SaveState() override;
		void LoadState() override;
		void FrameUpdate(const Common::Timer& timer) override;

		bool Render() const;

		void StartTracking();
		void TrackingUpdate(float positionX);
		void StopTracking();
		bool IsTracking() const { return m_tracking; }

	private:
		
		void Rotate(float radians);

	private:
		// Constant buffers must be 256-byte aligned.
		static const UINT c_alignedConstantBufferSize = (sizeof(ModelViewProjectionConstantBuffer) + 255) & ~255;

		// Cached pointer to device resources.
		std::shared_ptr<DirectX12Engine::DeviceResources> m_deviceResources;

		// Direct3D resources for cube geometry.
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	m_commandList;
		Microsoft::WRL::ComPtr<ID3D12RootSignature>			m_rootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState>			m_pipelineState;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>		m_cbvHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource>				m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource>				m_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource>				m_constantBuffer;
		ModelViewProjectionConstantBuffer					m_constantBufferData;
		UINT8*												m_mappedConstantBuffer;
		UINT												m_cbvDescriptorSize;
		D3D12_RECT											m_scissorRect;
		std::vector<byte>									m_vertexShader;
		std::vector<byte>									m_pixelShader;
		D3D12_VERTEX_BUFFER_VIEW							m_vertexBufferView;
		D3D12_INDEX_BUFFER_VIEW								m_indexBufferView;

		// Variables used with the rendering loop.
		bool	m_loadingComplete;
		float	m_radiansPerSecond;
		float	m_angle;
		bool	m_tracking;
	};
}

