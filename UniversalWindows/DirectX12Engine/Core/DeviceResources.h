#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>

#include "GraphicsEngineInterfaces/IWindow.h"
#include "Utilities/d3dx12.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

namespace DirectX12Engine
{
	static const UINT c_frameCount = 3;		// Use triple buffering.

	// Controls all the DirectX device resources.
	class DeviceResources
	{
	public:
		explicit DeviceResources(DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT);
		void SetWindow(const std::shared_ptr<GraphicsEngine::IWindow>& window);
		void SetLogicalSize(const DirectX::XMFLOAT2& logicalSize);
		void SetCurrentOrientation(GraphicsEngine::DisplayOrientations currentOrientation);
		void SetDpi(float dpi);
		void ValidateDevice();
		void Present();
		void WaitForGpu();

		// The size of the render target, in pixels.
		DirectX::XMFLOAT2	GetOutputSize() const				{ return m_outputSize; }
		float	GetAspectRatio() const { return m_outputSize.x / m_outputSize.y; }

		// The size of the render target, in dips.
		DirectX::XMFLOAT2	GetLogicalSize() const				{ return m_logicalSize; }

		float						GetDpi() const						{ return m_effectiveDpi; }
		bool						IsDeviceRemoved() const				{ return m_deviceRemoved; }

		// D3D Accessors.
		ID3D12Device*				GetD3DDevice() const				{ return m_d3dDevice.Get(); }
		IDXGISwapChain3*			GetSwapChain() const				{ return m_swapChain.Get(); }
		ID3D12Resource*				GetRenderTarget() const				{ return m_renderTargets[m_currentFrame].Get(); }
		ID3D12Resource*				GetDepthStencil() const				{ return m_depthStencil.Get(); }
		ID3D12CommandQueue*			GetCommandQueue() const				{ return m_commandQueue.Get(); }
		ID3D12CommandAllocator*		GetCommandAllocator() const			{ return m_commandAllocators[m_currentFrame].Get(); }
		DXGI_FORMAT					GetBackBufferFormat() const			{ return m_backBufferFormat; }
		DXGI_FORMAT					GetDepthBufferFormat() const		{ return m_depthBufferFormat; }
		D3D12_VIEWPORT				GetScreenViewport() const			{ return m_screenViewport; }
		DirectX::XMFLOAT4X4			GetOrientationTransform3D() const	{ return m_orientationTransform3D; }
		UINT						GetCurrentFrameIndex() const		{ return m_currentFrame; }

		CD3DX12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView() const
		{
			return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_currentFrame, m_rtvDescriptorSize);
		}
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const
		{
			return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
		}

		UINT GetCbvSrvUavDescriptorSize() const { return m_cbvSrvUavDescriptorSize; }
		UINT GetDsvDescriptorSize() const { return m_dsvDescriptorSize; }
		UINT GetRtvDescriptorSize() const { return m_rtvDescriptorSize; }

		UINT GetSampleCount() const { return m_sampleCount; }
		UINT GetSampleQuality() const { return m_sampleQuality; }

		static constexpr UINT GetFrameCount() { return c_frameCount; }

		Keyboard& Keyboard() { return m_keyboard; }
		Mouse& Mouse() { return m_mouse; }

	private:
		void CreateDeviceIndependentResources();
		void CreateDeviceResources();
		void CreateWindowSizeDependentResources();
		void UpdateRenderTargetSize();
		void MoveToNextFrame();
		DXGI_MODE_ROTATION ComputeDisplayRotation() const;
		void GetHardwareAdapter(IDXGIAdapter1** ppAdapter) const;

	private:
		void CreateDevice();
		void CreateSynchronizationObjects();
		void QueryDescriptorSizes();
		void QueryMultisampleQualityLevels();
		void CreateCommandObjects();
		void CreateSwapChain();
		void CreateDescriptorHeaps();
		void CreateRenderTargetView();
		void CreateDepthStencilBufferAndView();
		void SetViewport();

	private:
		UINT m_currentFrame;

		Microsoft::WRL::ComPtr<ID3D12Device> m_d3dDevice;
		Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgiFactory;
		Microsoft::WRL::ComPtr<IDXGISwapChain3>	m_swapChain;
		Microsoft::WRL::ComPtr<ID3D12Resource>	m_renderTargets[c_frameCount];
		Microsoft::WRL::ComPtr<ID3D12Resource>	m_depthStencil;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocators[c_frameCount];
		DXGI_FORMAT	m_backBufferFormat;
		DXGI_FORMAT	m_depthBufferFormat;
		D3D12_VIEWPORT m_screenViewport;
		UINT m_numMultisampleQualityLevels;
		bool m_deviceRemoved;

		std::shared_ptr<GraphicsEngine::IWindow> m_window;

		// CPU/GPU Synchronization.
		Microsoft::WRL::ComPtr<ID3D12Fence>	m_fence;
		UINT64 m_fenceValues[c_frameCount];
		HANDLE m_fenceEvent;

		// Cached device properties.
		DirectX::XMFLOAT2 m_d3dRenderTargetSize;
		DirectX::XMFLOAT2 m_outputSize;
		DirectX::XMFLOAT2 m_logicalSize;
		GraphicsEngine::DisplayOrientations	m_nativeOrientation;
		GraphicsEngine::DisplayOrientations	m_currentOrientation;
		float m_dpi;

		// This is the DPI that will be reported back to the app. It takes into account whether the app supports high resolution screens or not.
		float m_effectiveDpi;

		// Transforms used for display orientation.
		DirectX::XMFLOAT4X4	m_orientationTransform3D;

		UINT m_cbvSrvUavDescriptorSize = 0;
		UINT m_dsvDescriptorSize = 0;
		UINT m_rtvDescriptorSize = 0;

		UINT m_sampleCount = 1;
		UINT m_sampleQuality = 0;

		DirectX12Engine::Keyboard m_keyboard;
		DirectX12Engine::Mouse m_mouse;
	};
}
