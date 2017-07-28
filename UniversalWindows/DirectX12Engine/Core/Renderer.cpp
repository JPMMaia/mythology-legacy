#include "pch.h"
#include "Renderer.h"
#include "Scene/StandardScene.h"
#include "Utilities/DirectXHelper.h"
#include "Utilities/UploadBuffer.h"

using namespace Common;
using namespace DirectX;
using namespace DirectX12Engine;

Renderer::Renderer(const std::shared_ptr<DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources),
	m_rootSignatureManager(deviceResources),
	m_pipelineStateManager(deviceResources),
	m_commandListManager(deviceResources),
	m_scene(std::make_shared<StandardScene>(deviceResources, m_commandListManager))
{
	Renderer::CreateDeviceDependentResources();
	Renderer::CreateWindowSizeDependentResources();
}

void Renderer::CreateDeviceDependentResources()
{
	m_rootSignatureManager.CreateDeviceDependentResources();
	m_pipelineStateManager.CreateDeviceDependentResources(m_rootSignatureManager);
	m_scene->CreateDeviceDependentResources();

	m_rtvDescriptorHeap.CreateDeviceDependentResources(*m_deviceResources.get(), 2, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
	m_dsvDescriptorHeap.CreateDeviceDependentResources(*m_deviceResources.get(), 1, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
}
void Renderer::CreateWindowSizeDependentResources()
{
	auto viewport = m_deviceResources->GetScreenViewport();
	m_scissorRect = { 0, 0, static_cast<LONG>(viewport.Width), static_cast<LONG>(viewport.Height) };
	m_scene->CreateWindowSizeDependentResources();
	
	auto outputSize = m_deviceResources->GetOutputSize();
	
	// Fill RTV descriptor heap:
	{
		m_rtvDescriptorHeap.Clear();

		D3D12_CLEAR_VALUE clearValue = {};

		// Color:
		{
			clearValue.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			clearValue.Color[0] = 0.0f;
			clearValue.Color[1] = 0.0f;
			clearValue.Color[2] = 0.0f;
			clearValue.Color[3] = 1.0f;

			m_albedo = RWTexture();
			m_albedo.CreateWindowSizeDependentResources(
				*m_deviceResources.get(),
				static_cast<UINT64>(outputSize.x),
				static_cast<UINT64>(outputSize.y),
				clearValue.Format,
				D3D12_RESOURCE_STATE_COPY_SOURCE,
				&clearValue,
				D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
			);
			m_albedo.CreateRenderTargetView(*m_deviceResources.get(), m_rtvDescriptorHeap, "RTV");
		}

		// Normals:
		{
			/*clearValue.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

			m_normals = RWTexture();
			m_normals.CreateWindowSizeDependentResources(
				*m_deviceResources.get(),
				static_cast<UINT64>(outputSize.x),
				static_cast<UINT64>(outputSize.y),
				clearValue.Format,
				D3D12_RESOURCE_STATE_COPY_SOURCE,
				&clearValue,
				D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
			);*/
			
		}
	}

	// Fill DSV descriptor heap:
	{
		m_dsvDescriptorHeap.Clear();

		auto format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		D3D12_CLEAR_VALUE clearValue = {};
		clearValue.Format = format;
		clearValue.DepthStencil.Depth = 1.0f;
		clearValue.DepthStencil.Stencil = 0;

		m_depthStencil = RWTexture();
		m_depthStencil.CreateWindowSizeDependentResources(
			*m_deviceResources.get(), 
			static_cast<UINT64>(outputSize.x), 
			static_cast<UINT64>(outputSize.y), 
			format,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
		m_depthStencil.CreateDepthStencilView(*m_deviceResources.get(), m_dsvDescriptorHeap, "DSV");
	}
	
}

void Renderer::SaveState()
{
}
void Renderer::LoadState()
{
}

void Renderer::ProcessInput() const
{
	m_scene->ProcessInput();
}
void Renderer::FrameUpdate(const Common::Timer& timer)
{
	m_scene->FrameUpdate(timer);
}

bool Renderer::Render(const Common::Timer& timer)
{
	BeginRender();

	auto commandList = m_commandListManager.GetGraphicsCommandList(0);

	PIXBeginEvent(commandList, 0, L"Draw:");
	{
		// Set the graphics root signature:
		m_rootSignatureManager.SetGraphicsRootSignature(commandList, "RootSignature");

		// Set pipeline state:
		m_pipelineStateManager.SetPipelineState(commandList, "GBufferPass");

		// Render scene:
		m_scene->Render(timer);
	}
	PIXEndEvent(commandList);

	EndRender();

	return true;
}

void Renderer::BeginRender()
{
	auto commandAllocator = m_deviceResources->GetCommandAllocator();
	auto commandList = m_commandListManager.GetGraphicsCommandList(0);

	// Reset command allocator and command list:
	{
		// Reuse the memory associated with command recording:
		DX::ThrowIfFailed(commandAllocator->Reset());

		// Reuse command list:
		DX::ThrowIfFailed(commandList->Reset(commandAllocator, nullptr));
	}

	// Set the viewport and scissor rectangle.
	{
		D3D12_VIEWPORT viewport = m_deviceResources->GetScreenViewport();
		commandList->RSSetViewports(1, &viewport);
		commandList->RSSetScissorRects(1, &m_scissorRect);
	}

	// Indicate that the g-buffer will be used as render target:
	{
		auto renderTarget = m_albedo.GetResource();
		auto renderTargetResourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(renderTarget, D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
		commandList->ResourceBarrier(1, &renderTargetResourceBarrier);
	}

	// Clear and set render targets:
	{
		auto renderTargetView = m_albedo.CPUDescriptorHandle("RTV");
		auto depthStencilView = m_depthStencil.CPUDescriptorHandle("DSV");

		// Set render targets:
		commandList->OMSetRenderTargets(1, &renderTargetView, false, &depthStencilView);

		// Clear render target view:
		commandList->ClearRenderTargetView(renderTargetView, Colors::Black, 0, nullptr);

		// Clear depth stencil view:
		commandList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
	}
}
void Renderer::EndRender()
{
	auto commandList = m_commandListManager.GetGraphicsCommandList(0);

	// Indicate that the g-buffer will be used to as copy source:
	{
		auto renderTarget = m_albedo.GetResource();
		auto presentResourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(renderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_COPY_SOURCE);
		commandList->ResourceBarrier(1, &presentResourceBarrier);
	}

	// Indicate that the back buffer will be used as copy destination:
	{
		auto renderTarget = m_deviceResources->GetRenderTarget();
		auto renderTargetResourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(renderTarget, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_COPY_DEST);
		commandList->ResourceBarrier(1, &renderTargetResourceBarrier);
	}

	// Copy data from g-buffer to render target:
	commandList->CopyResource(m_deviceResources->GetRenderTarget(), m_albedo.GetResource());

	// Indicate that the back buffer will be used presented when the command list is executed:
	{
		auto renderTarget = m_deviceResources->GetRenderTarget();
		auto renderTargetResourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(renderTarget, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PRESENT);
		commandList->ResourceBarrier(1, &renderTargetResourceBarrier);
	}

	// Close command list:
	DX::ThrowIfFailed(commandList->Close());

	// Execute command list:
	m_commandListManager.ExecuteCommandList(0);
}
