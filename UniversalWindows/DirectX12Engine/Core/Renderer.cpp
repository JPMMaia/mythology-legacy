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
	m_rtvDescriptorHeap.CreateDeviceDependentResources(*m_deviceResources.get(), 1, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
}
void Renderer::CreateWindowSizeDependentResources()
{
	auto viewport = m_deviceResources->GetScreenViewport();
	m_scissorRect = { 0, 0, static_cast<LONG>(viewport.Width), static_cast<LONG>(viewport.Height) };
	m_scene->CreateWindowSizeDependentResources();
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

	// Indicate this resource will be in use as a render target:
	{
		auto renderTargetResourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetRenderTarget(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		commandList->ResourceBarrier(1, &renderTargetResourceBarrier);
	}

	// Clear and set render targets:
	{
		auto renderTargetView = m_deviceResources->GetRenderTargetView();
		auto depthStencilView = m_deviceResources->GetDepthStencilView();

		// Clear render target view:
		commandList->ClearRenderTargetView(renderTargetView, Colors::Black, 0, nullptr);

		// Clear depth stencil view:
		commandList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		// Set render targets:
		commandList->OMSetRenderTargets(1, &renderTargetView, false, &depthStencilView);
	}
}
void Renderer::EndRender()
{
	auto commandList = m_commandListManager.GetGraphicsCommandList(0);

	// Indicate that the render target will now be used to present when the command list is done executing:
	{
		CD3DX12_RESOURCE_BARRIER presentResourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetRenderTarget(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		commandList->ResourceBarrier(1, &presentResourceBarrier);
	}

	// Close command list:
	DX::ThrowIfFailed(commandList->Close());

	// Execute command list:
	m_commandListManager.ExecuteCommandList(0);
}
