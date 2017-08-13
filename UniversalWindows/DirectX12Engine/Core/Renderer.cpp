#include "pch.h"
#include "Renderer.h"
#include "Scene/StandardScene.h"
#include "Utilities/DirectXHelper.h"
#include "Utilities/UploadBuffer.h"
#include "Scene/RenderLayers.h"

using namespace Common;
using namespace DirectX;
using namespace DirectX12Engine;

Renderer::Renderer(const std::shared_ptr<DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources),
	m_rootSignatureManager(deviceResources),
	m_pipelineStateManager(deviceResources),
	m_commandListManager(deviceResources),
	m_clearColor{ 0.0f, 0.0f, 0.0f, 1.0f }
{
	Renderer::CreateDeviceDependentResources();
}

void Renderer::CreateDeviceDependentResources()
{
	m_rootSignatureManager.CreateDeviceDependentResources();
	m_pipelineStateManager.CreateDeviceDependentResources(m_rootSignatureManager);

	m_dsvDescriptorHeap.CreateDeviceDependentResources(m_deviceResources, 1, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
	m_rtvDescriptorHeap.CreateDeviceDependentResources(m_deviceResources, 3, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
	m_srvDescriptorHeap.CreateDeviceDependentResources(m_deviceResources, 3, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

	// Create graphics command list:
	ID3D12GraphicsCommandList* commandList;
	m_commandListManager.CreateGraphicsCommandList(commandList);
}
void Renderer::CreateWindowSizeDependentResources()
{
	auto viewport = m_deviceResources->GetScreenViewport();
	m_scissorRect = { 0, 0, static_cast<LONG>(viewport.Width), static_cast<LONG>(viewport.Height) };

	auto outputSize = m_deviceResources->GetOutputSize();

	m_dsvDescriptorHeap.Clear();
	m_rtvDescriptorHeap.Clear();
	m_srvDescriptorHeap.Clear();

	// G-Buffer:
	{
		// Positions:
		{
			auto format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			CD3DX12_CLEAR_VALUE clearValue(format, m_clearColor.data());

			auto& texture = m_positions;
			texture = Texture();
			texture.CreateResource(
				*m_deviceResources.get(),
				static_cast<UINT64>(outputSize.x),
				static_cast<UINT64>(outputSize.y),
				format,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				&clearValue,
				D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
			);
			texture.CreateRenderTargetView(m_deviceResources, m_rtvDescriptorHeap, "RTV");
			texture.CreateShaderResourceView(m_deviceResources, m_srvDescriptorHeap, "SRV");
			DX::SetName(texture.GetResource(), L"GBuffer Position");
		}

		// Albedo:
		{
			auto format = DXGI_FORMAT_B8G8R8A8_UNORM;
			CD3DX12_CLEAR_VALUE clearValue(format, m_clearColor.data());

			auto& texture = m_albedo;
			texture = Texture();
			texture.CreateResource(
				*m_deviceResources.get(),
				static_cast<UINT64>(outputSize.x),
				static_cast<UINT64>(outputSize.y),
				format,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				&clearValue,
				D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
			);
			texture.CreateRenderTargetView(m_deviceResources, m_rtvDescriptorHeap, "RTV");
			texture.CreateShaderResourceView(m_deviceResources, m_srvDescriptorHeap, "SRV");
			DX::SetName(texture.GetResource(), L"GBuffer Albedo");
		}

		// Normals:
		{
			auto format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			CD3DX12_CLEAR_VALUE clearValue(format, m_clearColor.data());

			auto& texture = m_normals;
			texture = Texture();
			texture.CreateResource(
				*m_deviceResources.get(),
				static_cast<UINT64>(outputSize.x),
				static_cast<UINT64>(outputSize.y),
				format,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				&clearValue,
				D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
			);
			texture.CreateRenderTargetView(m_deviceResources, m_rtvDescriptorHeap, "RTV");
			texture.CreateShaderResourceView(m_deviceResources, m_srvDescriptorHeap, "SRV");
			DX::SetName(texture.GetResource(), L"GBuffer Normal");
		}
	}

	// Fill DSV descriptor heap:
	{
		auto format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		CD3DX12_CLEAR_VALUE clearValue(format, 1.0f, 0);

		m_depthStencil = Texture();
		m_depthStencil.CreateResource(
			*m_deviceResources.get(),
			static_cast<UINT64>(outputSize.x),
			static_cast<UINT64>(outputSize.y),
			format,
			D3D12_RESOURCE_STATE_DEPTH_WRITE,
			&clearValue,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
		m_depthStencil.CreateDepthStencilView(m_deviceResources, m_dsvDescriptorHeap, "DSV");
		DX::SetName(m_depthStencil.GetResource(), L"Renderer Depth Stencil");
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
}
void Renderer::FrameUpdate(const Common::Timer& timer)
{
	m_scene->FrameUpdate(timer);
}

bool Renderer::Render(const Common::Timer& timer)
{
	BeginRender();

	auto commandList = m_commandListManager.GetGraphicsCommandList(0);

	// Clear and set render targets for G-Buffer pass:
	PIXBeginEvent(commandList, 0, L"Begin G-Buffer Pass");
	{
		// Indicate that the G-Buffer textures will be used as Render Targets:
		{
			std::array<CD3DX12_RESOURCE_BARRIER, 3> barriers =
			{
				CD3DX12_RESOURCE_BARRIER::Transition(m_positions.GetResource(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET),
				CD3DX12_RESOURCE_BARRIER::Transition(m_albedo.GetResource(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET),
				CD3DX12_RESOURCE_BARRIER::Transition(m_normals.GetResource(), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET),
			};
			commandList->ResourceBarrier(static_cast<UINT>(barriers.size()), barriers.data());
		}

		std::array<D3D12_CPU_DESCRIPTOR_HANDLE, 3> renderTargetViews =
		{
			m_positions.CPUDescriptorHandle("RTV"),
			m_albedo.CPUDescriptorHandle("RTV"),
			m_normals.CPUDescriptorHandle("RTV")
		};
		auto depthStencilView = m_depthStencil.CPUDescriptorHandle("DSV");

		// Set render targets:
		commandList->OMSetRenderTargets(static_cast<UINT>(renderTargetViews.size()), renderTargetViews.data(), true, &depthStencilView);

		// Clear render target views:
		std::for_each(renderTargetViews.begin(), renderTargetViews.end(), [this, &commandList](auto renderTargetView)
		{
			commandList->ClearRenderTargetView(renderTargetView, m_clearColor.data(), 0, nullptr);
		});

		// Clear depth stencil view:
		commandList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		// Set the graphics root signature:
		m_rootSignatureManager.SetGraphicsRootSignature(commandList, "GBufferPass");
	}
	PIXEndEvent(commandList);

	PIXBeginEvent(commandList, 0, L"Draw");
	{
		// Set pipeline state:
		m_pipelineStateManager.SetPipelineState(commandList, "GBufferPass");

		// Render scene:
		m_scene->Render(timer, RenderLayer::Opaque);
	}
	PIXEndEvent(commandList);

	PIXBeginEvent(commandList, 0, L"Begin Lighting Pass");
	{
		// Set the graphics root signature:
		m_rootSignatureManager.SetGraphicsRootSignature(commandList, "LightingPass");

		// Indicate that the G-Buffer textures will be used as Pixel Shader Resources:
		{
			std::array<CD3DX12_RESOURCE_BARRIER, 3> barriers =
			{
				CD3DX12_RESOURCE_BARRIER::Transition(m_positions.GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE),
				CD3DX12_RESOURCE_BARRIER::Transition(m_albedo.GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE),
				CD3DX12_RESOURCE_BARRIER::Transition(m_normals.GetResource(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE),
			};
			commandList->ResourceBarrier(static_cast<UINT>(barriers.size()), barriers.data());
		}

		// Set render targets for Lighting Pass:
		auto renderTargetView = m_deviceResources->GetRenderTargetView();
		auto depthStencilView = m_depthStencil.CPUDescriptorHandle("DSV");
		commandList->OMSetRenderTargets(1, &renderTargetView, false, &depthStencilView);

		// Set G-Buffer textures:
		std::array<ID3D12DescriptorHeap*, 1> descriptorHeaps = { m_srvDescriptorHeap.Get() };
		commandList->SetDescriptorHeaps(static_cast<UINT>(descriptorHeaps.size()), descriptorHeaps.data());
		commandList->SetGraphicsRootDescriptorTable(3, m_srvDescriptorHeap.Get()->GetGPUDescriptorHandleForHeapStart());
	}
	PIXEndEvent(commandList);

	PIXBeginEvent(commandList, 0, L"Draw");
	{
		// Set pipeline state:
		m_pipelineStateManager.SetPipelineState(commandList, "LightingPass");

		// Render scene:
		m_scene->Render(timer, RenderLayer::LightingPass);
	}
	PIXEndEvent(commandList);

	EndRender();

	return true;
}

void Renderer::SetScene(const std::shared_ptr<IScene>& scene)
{
	m_scene = scene;
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

	// Indicate that the back buffer will be used as render target:
	{
		auto renderTarget = m_deviceResources->GetRenderTarget();
		auto renderTargetResourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(renderTarget, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		commandList->ResourceBarrier(1, &renderTargetResourceBarrier);
	}

	// Set the viewport and scissor rectangle.
	{
		auto viewport = m_deviceResources->GetScreenViewport();
		commandList->RSSetViewports(1, &viewport);
		commandList->RSSetScissorRects(1, &m_scissorRect);
	}
}
void Renderer::EndRender()
{
	auto commandList = m_commandListManager.GetGraphicsCommandList(0);

	// Indicate that the back buffer will be presented when the command list is executed:
	{
		auto renderTarget = m_deviceResources->GetRenderTarget();
		auto renderTargetResourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(renderTarget, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		commandList->ResourceBarrier(1, &renderTargetResourceBarrier);
	}

	// Close command list:
	DX::ThrowIfFailed(commandList->Close());

	// Execute command list:
	m_commandListManager.ExecuteCommandList(0);
}
