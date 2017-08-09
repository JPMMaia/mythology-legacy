#include "pch.h"
#include "DeviceResources.h"
#include "Utilities/DirectXHelper.h"
#include "GraphicsEngineInterfaces/DisplayOrientations.h"

using namespace Eigen;
using namespace DirectX;
using namespace DirectX12Engine;
using namespace Microsoft::WRL;
using namespace Windows::Foundation;

namespace DisplayMetrics
{
	// High resolution displays can require a lot of GPU and battery power to render.
	// High resolution phones, for example, may suffer from poor battery life if
	// games attempt to render at 60 frames per second at full fidelity.
	// The decision to render at full fidelity across all platforms and form factors
	// should be deliberate.
	static const bool SupportHighResolutions = false;

	// The default thresholds that define a "high resolution" display. If the thresholds
	// are exceeded and SupportHighResolutions is false, the dimensions will be scaled
	// by 50%.
	static const float DpiThreshold = 192.0f;		// 200% of standard desktop display.
	static const float WidthThreshold = 1920.0f;	// 1080p width.
	static const float HeightThreshold = 1080.0f;	// 1080p height.
};

class ScreenRotation
{
public:
	static void Initialize()
	{
		s_rotation0 = Matrix4f::Identity();

		s_rotation90 <<
			0.0f, 1.0f, 0.0f, 0.0f,
			-1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f;

		s_rotation180 <<
			-1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f;

		s_rotation270 <<
			0.0f, -1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f;

		s_initialized = true;
	}

	static const Matrix4f& GetRotation0()
	{
		if (!s_initialized)
			Initialize();

		return s_rotation0;
	}
	static const Matrix4f& GetRotation90()
	{
		if (!s_initialized)
			Initialize();

		return s_rotation90;
	}
	static const Matrix4f& GetRotation180()
	{
		if (!s_initialized)
			Initialize();

		return s_rotation180;
	}
	static const Matrix4f& GetRotation270()
	{
		if (!s_initialized)
			Initialize();

		return s_rotation270;
	}

private:
	static Matrix4f s_rotation0;
	static Matrix4f s_rotation90;
	static Matrix4f s_rotation180;
	static Matrix4f s_rotation270;
	static bool s_initialized;
};

Matrix4f ScreenRotation::s_rotation0;
Matrix4f ScreenRotation::s_rotation90;
Matrix4f ScreenRotation::s_rotation180;
Matrix4f ScreenRotation::s_rotation270;
bool ScreenRotation::s_initialized(false);

DeviceResources::DeviceResources(DXGI_FORMAT backBufferFormat, DXGI_FORMAT depthBufferFormat) :
	m_currentFrame(0),
	m_backBufferFormat(backBufferFormat),
	m_depthBufferFormat(depthBufferFormat),
	m_screenViewport(),
	m_deviceRemoved(false),
	m_fenceValues{},
	m_fenceEvent(nullptr),
	m_d3dRenderTargetSize(),
	m_outputSize(),
	m_logicalSize(),
	m_nativeOrientation(GraphicsEngine::DisplayOrientations::None),
	m_currentOrientation(GraphicsEngine::DisplayOrientations::None),
	m_dpi(-1.0f),
	m_effectiveDpi(-1.0f),
	m_rtvDescriptorSize(0)
{
	CreateDeviceIndependentResources();
	CreateDeviceResources();
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppMemberFunctionMayBeConst
void DeviceResources::CreateDeviceIndependentResources()
{
}
void DeviceResources::CreateDeviceResources()
{
#if defined(_DEBUG)
	// If the project is in a debug build, enable debugging via SDK Layers:
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			debugController->EnableDebugLayer();
	}
#endif

	CreateDevice();
	QueryDescriptorSizes();
	QueryMultisampleQualityLevels();
	CreateCommandObjects();
	CreateDescriptorHeaps();
	CreateSynchronizationObjects();
}
void DeviceResources::CreateWindowSizeDependentResources()
{
	// Wait until all previous GPU work is complete:
	WaitForGpu();

	// Clear the previous window size specific content and update the tracked fence values:
	for (UINT n = 0; n < c_frameCount; n++)
	{
		m_renderTargets[n] = nullptr;
		m_fenceValues[n] = m_fenceValues[m_currentFrame];
	}

	UpdateRenderTargetSize();
	CreateSwapChain();
	CreateRenderTargetView();
	CreateDepthStencilBufferAndView();
	SetViewport();
}

void DeviceResources::UpdateRenderTargetSize()
{
	m_effectiveDpi = m_dpi;

	// To improve battery life on high resolution devices, render to a smaller render target
	// and allow the GPU to scale the output when it is presented.
	// ReSharper disable once CppRedundantBooleanExpressionArgument
	if (!DisplayMetrics::SupportHighResolutions && m_dpi > DisplayMetrics::DpiThreshold)
	{
		float width = DX::ConvertDipsToPixels(m_logicalSize.x, m_dpi);
		float height = DX::ConvertDipsToPixels(m_logicalSize.y, m_dpi);

		// When the device is in portrait orientation, height > width. Compare the
		// larger dimension against the width threshold and the smaller dimension
		// against the height threshold.
		if (max(width, height) > DisplayMetrics::WidthThreshold && min(width, height) > DisplayMetrics::HeightThreshold)
		{
			// To scale the app we change the effective DPI. Logical size does not change.
			m_effectiveDpi /= 2.0f;
		}
	}

	// Calculate the necessary render target size in pixels:
	m_outputSize.x = DX::ConvertDipsToPixels(m_logicalSize.x, m_effectiveDpi);
	m_outputSize.y = DX::ConvertDipsToPixels(m_logicalSize.y, m_effectiveDpi);

	// Prevent zero size DirectX content from being created:
	m_outputSize.x = max(m_outputSize.x, 1.0f);
	m_outputSize.y = max(m_outputSize.y, 1.0f);
}
void DeviceResources::SetWindow(const std::shared_ptr<GraphicsEngine::IWindow>& window)
{
	m_window = window;
	m_logicalSize = window->Size();
	m_nativeOrientation = window->NativeOrientation();
	m_currentOrientation = window->CurrentOrientation();
	m_dpi = window->LogicalDpi();

	CreateWindowSizeDependentResources();
}

void DeviceResources::SetLogicalSize(const DirectX::XMFLOAT2& logicalSize)
{
	if (m_logicalSize.x != logicalSize.x || m_logicalSize.y != logicalSize.y)
	{
		m_logicalSize = logicalSize;
		CreateWindowSizeDependentResources();
	}
}
void DeviceResources::SetDpi(float dpi)
{
	if (dpi != m_dpi)
	{
		m_dpi = dpi;

		// When the display DPI changes, the logical size of the window (measured in Dips) also changes and needs to be updated.
		m_logicalSize = m_window->Size();

		CreateWindowSizeDependentResources();
	}
}
void DeviceResources::SetCurrentOrientation(GraphicsEngine::DisplayOrientations currentOrientation)
{
	if (m_currentOrientation != currentOrientation)
	{
		m_currentOrientation = currentOrientation;
		CreateWindowSizeDependentResources();
	}
}

void DeviceResources::ValidateDevice()
{
	// The D3D Device is no longer valid if the default adapter changed since the device
	// was created or if the device has been removed.

	// First, get the LUID for the default adapter from when the device was created.

	DXGI_ADAPTER_DESC previousDesc;
	{
		ComPtr<IDXGIAdapter1> previousDefaultAdapter;
		DX::ThrowIfFailed(m_dxgiFactory->EnumAdapters1(0, &previousDefaultAdapter));

		DX::ThrowIfFailed(previousDefaultAdapter->GetDesc(&previousDesc));
	}

	// Next, get the information for the current default adapter.

	DXGI_ADAPTER_DESC currentDesc;
	{
		ComPtr<IDXGIFactory4> currentDxgiFactory;
		DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&currentDxgiFactory)));

		ComPtr<IDXGIAdapter1> currentDefaultAdapter;
		DX::ThrowIfFailed(currentDxgiFactory->EnumAdapters1(0, &currentDefaultAdapter));

		DX::ThrowIfFailed(currentDefaultAdapter->GetDesc(&currentDesc));
	}

	// If the adapter LUIDs don't match, or if the device reports that it has been removed,
	// a new D3D device must be created.

	if (previousDesc.AdapterLuid.LowPart != currentDesc.AdapterLuid.LowPart ||
		previousDesc.AdapterLuid.HighPart != currentDesc.AdapterLuid.HighPart ||
		FAILED(m_d3dDevice->GetDeviceRemovedReason()))
	{
		m_deviceRemoved = true;
	}
}

void DeviceResources::Present()
{
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	HRESULT hr = m_swapChain->Present(1, 0);

	// If the device was removed either by a disconnection or a driver upgrade, we 
	// must recreate all device resources.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		m_deviceRemoved = true;
	}
	else
	{
		DX::ThrowIfFailed(hr);

		MoveToNextFrame();
	}
}
void DeviceResources::WaitForGpu()
{
	// Schedule a Signal command in the queue.
	DX::ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), m_fenceValues[m_currentFrame]));

	// Wait until the fence has been crossed.
	DX::ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_currentFrame], m_fenceEvent));
	WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);

	// Increment the fence value for the current frame.
	m_fenceValues[m_currentFrame]++;
}
void DeviceResources::MoveToNextFrame()
{
	// Schedule a Signal command in the queue.
	const UINT64 currentFenceValue = m_fenceValues[m_currentFrame];
	DX::ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), currentFenceValue));

	// Advance the frame index.
	m_currentFrame = m_swapChain->GetCurrentBackBufferIndex();

	// Check to see if the next frame is ready to start.
	if (m_fence->GetCompletedValue() < m_fenceValues[m_currentFrame])
	{
		DX::ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_currentFrame], m_fenceEvent));
		WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
	}

	// Set the fence value for the next frame.
	m_fenceValues[m_currentFrame] = currentFenceValue + 1;
}

DXGI_MODE_ROTATION DeviceResources::ComputeDisplayRotation() const
{
	DXGI_MODE_ROTATION rotation = DXGI_MODE_ROTATION_UNSPECIFIED;

	// Note: NativeOrientation can only be Landscape or Portrait even though
	// the DisplayOrientations enum has other values.
	// ReSharper disable once CppIncompleteSwitchStatement
	// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
	switch (m_nativeOrientation)
	{
	case GraphicsEngine::DisplayOrientations::Landscape:
		// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
		// ReSharper disable once CppIncompleteSwitchStatement
		switch (m_currentOrientation)
		{
		case GraphicsEngine::DisplayOrientations::Landscape:
			rotation = DXGI_MODE_ROTATION_IDENTITY;
			break;

		case GraphicsEngine::DisplayOrientations::Portrait:
			rotation = DXGI_MODE_ROTATION_ROTATE270;
			break;

		case GraphicsEngine::DisplayOrientations::LandscapeFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE180;
			break;

		case GraphicsEngine::DisplayOrientations::PortraitFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE90;
			break;
		}
		break;

	case GraphicsEngine::DisplayOrientations::Portrait:
		// ReSharper disable once CppIncompleteSwitchStatement
		// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
		switch (m_currentOrientation)
		{
		case GraphicsEngine::DisplayOrientations::Landscape:
			rotation = DXGI_MODE_ROTATION_ROTATE90;
			break;

		case GraphicsEngine::DisplayOrientations::Portrait:
			rotation = DXGI_MODE_ROTATION_IDENTITY;
			break;

		case GraphicsEngine::DisplayOrientations::LandscapeFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE270;
			break;

		case GraphicsEngine::DisplayOrientations::PortraitFlipped:
			rotation = DXGI_MODE_ROTATION_ROTATE180;
			break;
		}
		break;
	}
	return rotation;
}

void DeviceResources::GetHardwareAdapter(IDXGIAdapter1** ppAdapter) const
{
	ComPtr<IDXGIAdapter1> adapter;
	*ppAdapter = nullptr;

	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != m_dxgiFactory->EnumAdapters1(adapterIndex, &adapter); adapterIndex++)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Don't select the Basic Render Driver adapter.
			continue;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the
		// actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}

	*ppAdapter = adapter.Detach();
}

void DeviceResources::CreateDevice()
{
	// Create factory:
	DX::ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory)));

	// Get adapter:
	ComPtr<IDXGIAdapter1> adapter;
	GetHardwareAdapter(&adapter);

	// Create device:
	static constexpr auto minimunFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	auto hr = D3D12CreateDevice(
		adapter.Get(),
		minimunFeatureLevel,
		IID_PPV_ARGS(&m_d3dDevice)
	);

#if defined(_DEBUG)

	// If the initialization fails, fall back to the WARP device:
	if (FAILED(hr))
	{
		ComPtr<IDXGIAdapter> warpAdapter;
		DX::ThrowIfFailed(m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

		hr = D3D12CreateDevice(warpAdapter.Get(), minimunFeatureLevel, IID_PPV_ARGS(&m_d3dDevice));
	}

#endif

	DX::ThrowIfFailed(hr);
}
void DeviceResources::CreateSynchronizationObjects()
{
	DX::ThrowIfFailed(m_d3dDevice->CreateFence(m_fenceValues[m_currentFrame], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
	m_fenceValues[m_currentFrame]++;

	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (m_fenceEvent == nullptr)
	{
		DX::ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
	}
}
void DeviceResources::QueryDescriptorSizes()
{
	m_rtvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_dsvDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_cbvSrvUavDescriptorSize = m_d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}
void DeviceResources::QueryMultisampleQualityLevels()
{
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS multisampleQualityLevels;
	multisampleQualityLevels.Format = m_backBufferFormat;
	multisampleQualityLevels.SampleCount = 4;
	multisampleQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	multisampleQualityLevels.NumQualityLevels = 0;

	DX::ThrowIfFailed(
		m_d3dDevice->CheckFeatureSupport(
			D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
			&multisampleQualityLevels,
			sizeof(multisampleQualityLevels)
		)
	);

	m_numMultisampleQualityLevels = multisampleQualityLevels.NumQualityLevels;
	assert(m_numMultisampleQualityLevels > 0 && "Unexpected MSAA quality level.");
}
void DeviceResources::CreateCommandObjects()
{
	// Create command queue:
	{
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		DX::ThrowIfFailed(m_d3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));
		NAME_D3D12_OBJECT(m_commandQueue);
	}

	// Create command allocator:
	for (UINT n = 0; n < c_frameCount; n++)
	{
		DX::ThrowIfFailed(
			m_d3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocators[n]))
		);
	}
}
void DeviceResources::CreateSwapChain()
{
	auto displayRotation = ComputeDisplayRotation();

	// Compute the size of the render target:
	{
		auto swapDimensions = displayRotation == DXGI_MODE_ROTATION_ROTATE90 || displayRotation == DXGI_MODE_ROTATION_ROTATE270;
		m_d3dRenderTargetSize.x = swapDimensions ? m_outputSize.y : m_outputSize.x;
		m_d3dRenderTargetSize.y = swapDimensions ? m_outputSize.x : m_outputSize.y;
	}

	// Create swap chain:
	{
		auto backBufferWidth = lround(m_d3dRenderTargetSize.x);
		auto backBufferHeight = lround(m_d3dRenderTargetSize.y);

		// If the swap chain already exists:
		if (m_swapChain != nullptr)
		{
			// Resize:
			auto hr = m_swapChain->ResizeBuffers(c_frameCount, backBufferWidth, backBufferHeight, m_backBufferFormat, 0);

			// If the device was removed for any reason, a new device and swap chain will need to be created:
			if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
			{
				m_deviceRemoved = true;
				return;
			}

			DX::ThrowIfFailed(hr);
		}
		// Otherwise, create a new one using the same adapter as the existing Direct3D device:
		else
		{
			// ReSharper disable once CppUnreachableCode
			auto scaling = DisplayMetrics::SupportHighResolutions ? DXGI_SCALING_NONE : DXGI_SCALING_STRETCH;

			DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
			swapChainDesc.Width = backBufferWidth;
			swapChainDesc.Height = backBufferHeight;
			swapChainDesc.Format = m_backBufferFormat;
			swapChainDesc.Stereo = false;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = c_frameCount;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	// All Windows Universal apps must use _FLIP_ SwapEffects.
			swapChainDesc.Flags = 0;
			swapChainDesc.Scaling = scaling;
			swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

			ComPtr<IDXGISwapChain1> swapChain;
			DX::ThrowIfFailed(
				m_dxgiFactory->CreateSwapChainForCoreWindow(
					m_commandQueue.Get(),
					reinterpret_cast<IUnknown*>(m_window->Get()),
					&swapChainDesc,
					nullptr,
					&swapChain
				)
			);

			DX::ThrowIfFailed(swapChain.As(&m_swapChain));
		}
	}

	// Set the proper orientation for the swap chain, and generate
	// 3D matrix transformations for rendering to the rotated swap chain.
	// The 3D matrix is specified explicitly to avoid rounding errors.
	{
		switch (displayRotation)
		{
		case DXGI_MODE_ROTATION_IDENTITY:
			m_orientationTransform3D = ScreenRotation::GetRotation0();
			break;

		case DXGI_MODE_ROTATION_ROTATE90:
			m_orientationTransform3D = ScreenRotation::GetRotation270();
			break;

		case DXGI_MODE_ROTATION_ROTATE180:
			m_orientationTransform3D = ScreenRotation::GetRotation180();
			break;

		case DXGI_MODE_ROTATION_ROTATE270:
			m_orientationTransform3D = ScreenRotation::GetRotation90();
			break;

		default:
			throw std::exception();
		}

		DX::ThrowIfFailed(
			m_swapChain->SetRotation(displayRotation)
		);
	}
}
void DeviceResources::CreateDescriptorHeaps()
{
	// Create RTV descriptor heap:
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = c_frameCount + 1;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		DX::ThrowIfFailed(m_d3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));
		NAME_D3D12_OBJECT(m_rtvHeap);
	}

	// Create DSV descriptor heaps:
	{
		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		DX::ThrowIfFailed(m_d3dDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap)));
		NAME_D3D12_OBJECT(m_dsvHeap);
	}
}
void DeviceResources::CreateRenderTargetView()
{
	m_currentFrame = m_swapChain->GetCurrentBackBufferIndex();

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvDescriptor(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT n = 0; n < c_frameCount; n++)
	{
		DX::ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
		m_d3dDevice->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvDescriptor);
		rtvDescriptor.Offset(m_rtvDescriptorSize);

		WCHAR name[25];
		if (swprintf_s(name, L"m_renderTargets[%u]", n) > 0)
		{
			DX::SetName(m_renderTargets[n].Get(), name);
		}
	}
}
void DeviceResources::CreateDepthStencilBufferAndView()
{
	// Create DSV buffer:
	{
		D3D12_HEAP_PROPERTIES depthHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		auto backBufferWidth = lround(m_d3dRenderTargetSize.x);
		auto backBufferHeight = lround(m_d3dRenderTargetSize.y);
		D3D12_RESOURCE_DESC depthResourceDesc = CD3DX12_RESOURCE_DESC::Tex2D(m_depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1);
		depthResourceDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

		CD3DX12_CLEAR_VALUE depthOptimizedClearValue(m_depthBufferFormat, 1.0f, 0);

		DX::ThrowIfFailed(m_d3dDevice->CreateCommittedResource(
			&depthHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&depthResourceDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&depthOptimizedClearValue,
			IID_PPV_ARGS(&m_depthStencil)
		));
		NAME_D3D12_OBJECT(m_depthStencil);
	}

	// Create DSV view:
	{
		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = m_depthBufferFormat;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Flags = D3D12_DSV_FLAG_NONE;

		m_d3dDevice->CreateDepthStencilView(m_depthStencil.Get(), &dsvDesc, m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
	}
}
void DeviceResources::SetViewport()
{
	// Set the viewport:
	{
		m_screenViewport = {};
		m_screenViewport.TopLeftX = 0.0f;
		m_screenViewport.TopLeftY = 0.0f;
		m_screenViewport.Width = m_d3dRenderTargetSize.x;
		m_screenViewport.Height = m_d3dRenderTargetSize.y;
		m_screenViewport.MinDepth = 0.0f;
		m_screenViewport.MaxDepth = 1.0f;
	}
}
