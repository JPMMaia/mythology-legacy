#include "pch.h"
#include "Renderer.h"
#include "Scene/StandardScene.h"
#include "Utilities/DirectXHelper.h"
#include "Utilities/UploadBuffer.h"

using namespace Common;
using namespace DirectX;
using namespace GraphicsEngine;

Renderer::Renderer(const std::shared_ptr<DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources),
	m_rootSignatureManager(deviceResources),
	m_pipelineStateManager(deviceResources),
	m_commandListManager(m_deviceResources),
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

void Renderer::FrameUpdate(const Common::Timer& timer)
{
	m_scene->FrameUpdate(timer);
}

void Renderer::FixedUpdate(const Common::Timer& timer)
{
}

bool Renderer::Render(const Common::Timer& timer)
{
	Helpers::ThrowIfFailed(m_deviceResources->GetCommandAllocator()->Reset());

	auto commandList = m_commandListManager.GetGraphicsCommandList(0);

	// The command list can be reset anytime after ExecuteCommandList() is called.
	Helpers::ThrowIfFailed(commandList->Reset(m_deviceResources->GetCommandAllocator(), nullptr));

	PIXBeginEvent(commandList, 0, L"Draw the cube");
	{
		// Set the graphics root signature:
		m_rootSignatureManager.SetGraphicsRootSignature(commandList, "RootSignature");

		// Set the viewport and scissor rectangle.
		D3D12_VIEWPORT viewport = m_deviceResources->GetScreenViewport();
		commandList->RSSetViewports(1, &viewport);
		commandList->RSSetScissorRects(1, &m_scissorRect);

		// Indicate this resource will be in use as a render target:
		CD3DX12_RESOURCE_BARRIER renderTargetResourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetRenderTarget(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		commandList->ResourceBarrier(1, &renderTargetResourceBarrier);

		// Record drawing commands.
		D3D12_CPU_DESCRIPTOR_HANDLE renderTargetView = m_deviceResources->GetRenderTargetView();
		D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = m_deviceResources->GetDepthStencilView();
		commandList->ClearRenderTargetView(renderTargetView, DirectX::Colors::CornflowerBlue, 0, nullptr);
		commandList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		commandList->OMSetRenderTargets(1, &renderTargetView, false, &depthStencilView);

		auto cube = dynamic_cast<StandardScene*>(m_scene.get())->GetCubeRenderItem();
		cube->Render(commandList);

		// Indicate that the render target will now be used to present when the command list is done executing:
		CD3DX12_RESOURCE_BARRIER presentResourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(m_deviceResources->GetRenderTarget(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		commandList->ResourceBarrier(1, &presentResourceBarrier);
	}
	PIXEndEvent(commandList);

	Helpers::ThrowIfFailed(commandList->Close());

	m_commandListManager.ExecuteCommandList(0);

	return true;
}
