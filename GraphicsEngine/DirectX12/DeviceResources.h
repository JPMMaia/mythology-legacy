﻿#pragma once

#include "GraphicsEngine/DirectX12/Utilities/d3dx12.h"
#include "GraphicsEngine/Interfaces/IWindow.h"
#include "GraphicsEngine/Interfaces/ISwapChainPanel.h"

namespace GraphicsEngine
{
	enum class DisplayOrientations;
	static const UINT c_frameCount = 3;		// Use triple buffering.

	// Controls all the DirectX device resources.
	class DeviceResources
	{
	public:
		explicit DeviceResources(DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D32_FLOAT);
		void SetSwapChainPanel(std::unique_ptr<ISwapChainPanel>&& swapChainPanel);
		void SetLogicalSize(const DirectX::XMFLOAT2& logicalSize);
		void SetCurrentOrientation(DisplayOrientations currentOrientation);
		void SetDpi(float dpi);
		void SetCompositionScale(float compositionScaleX, float compositionScaleY);
		void ValidateDevice();
		void Present();
		void WaitForGpu();

		// The size of the render target, in pixels.
		const DirectX::XMFLOAT2&   GetOutputSize() const { return m_outputSize; }

		// The size of the render target, in dips.
		const DirectX::XMFLOAT2&   GetLogicalSize() const { return m_logicalSize; }

		float                       GetDpi() const { return m_effectiveDpi; }
		bool                        IsDeviceRemoved() const { return m_deviceRemoved; }

		// D3D Accessors.
		ID3D12Device*               GetD3DDevice() const { return m_d3dDevice.Get(); }
		IDXGISwapChain3*            GetSwapChain() const { return m_swapChain.Get(); }
		ID3D12Resource*             GetRenderTarget() const { return m_renderTargets[m_currentFrame].Get(); }
		ID3D12Resource*             GetDepthStencil() const { return m_depthStencil.Get(); }
		ID3D12CommandQueue*         GetCommandQueue() const { return m_commandQueue.Get(); }
		ID3D12CommandAllocator*     GetCommandAllocator() const { return m_commandAllocators[m_currentFrame].Get(); }
		DXGI_FORMAT                 GetBackBufferFormat() const { return m_backBufferFormat; }
		DXGI_FORMAT                 GetDepthBufferFormat() const { return m_depthBufferFormat; }
		D3D12_VIEWPORT              GetScreenViewport() const { return m_screenViewport; }
		DirectX::XMFLOAT4X4         GetOrientationTransform3D() const { return m_orientationTransform3D; }
		UINT                        GetCurrentFrameIndex() const { return m_currentFrame; }
		UINT						GetSampleCount() const { return 1; }
		UINT						GetSampleQuality() const { return 0; }

		CD3DX12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView() const
		{
			return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_currentFrame, m_rtvDescriptorSize);
		}
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const
		{
			return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
		}

	private:
		void CreateDeviceIndependentResources();
		void CreateDeviceResources();
		void CreateWindowSizeDependentResources();
		void UpdateRenderTargetSize();
		void SetSwapChainOnPanelAsync(IDXGISwapChain3* swapChain);
		void RemoveDevice();
		void MoveToNextFrame();
		DXGI_MODE_ROTATION ComputeDisplayRotation();
		void GetHardwareAdapter(IDXGIAdapter1** ppAdapter);

		UINT m_currentFrame;

		// Direct3D objects.
		Microsoft::WRL::ComPtr<ID3D12Device> m_d3dDevice;
		Microsoft::WRL::ComPtr<IDXGIFactory4> m_dxgiFactory;
		Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[c_frameCount];
		Microsoft::WRL::ComPtr<ID3D12Resource> m_depthStencil;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocators[c_frameCount];
		DXGI_FORMAT m_backBufferFormat;
		DXGI_FORMAT m_depthBufferFormat;
		D3D12_VIEWPORT m_screenViewport;
		UINT m_rtvDescriptorSize;
		bool m_deviceRemoved;

		// CPU/GPU Synchronization.
		Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
		UINT64 m_fenceValues[c_frameCount];
		HANDLE m_fenceEvent;

		// Cached reference to the XAML panel.
		std::unique_ptr<ISwapChainPanel> m_swapChainPanel;

		// Cached device properties.
		DirectX::XMFLOAT2 m_d3dRenderTargetSize;
		DirectX::XMFLOAT2 m_outputSize;
		DirectX::XMFLOAT2 m_logicalSize;
		DisplayOrientations m_nativeOrientation;
		DisplayOrientations m_currentOrientation;
		float m_dpi;
		float m_compositionScaleX;
		float m_compositionScaleY;

		// This is the DPI that will be reported back to the app. It takes into account whether the app supports high resolution screens or not.
		float m_effectiveDpi;

		// Transforms used for display orientation.
		DirectX::XMFLOAT4X4 m_orientationTransform3D;
	};
}
