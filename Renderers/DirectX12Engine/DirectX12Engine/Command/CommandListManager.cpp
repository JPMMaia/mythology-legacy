#include "pch.h"
#include "CommandListManager.h"
#include "Common/Helpers.h"
#include "DirectX12Engine/Utilities/DirectXHelper.h"

using namespace Common;
using namespace DirectX12Engine;
using namespace Microsoft::WRL;

CommandListManager::CommandListManager(const std::shared_ptr<DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
}

size_t CommandListManager::CreateGraphicsCommandList(ID3D12GraphicsCommandList*& graphicsCommandList)
{
	auto d3dDevice = m_deviceResources->GetD3DDevice();

	ComPtr<ID3D12GraphicsCommandList> commandList;
	DX::ThrowIfFailed(d3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_deviceResources->GetCommandAllocator(), nullptr, IID_PPV_ARGS(&commandList)));
	graphicsCommandList = commandList.Get();
	
	{
		auto name = L"Graphics Command List " + std::to_wstring(m_graphicsCommandLists.size());
		DX::SetName(commandList.Get(), name.c_str());
	}
	
	m_graphicsCommandLists.push_back(std::move(commandList));

	return m_graphicsCommandLists.size() - 1;
}
void CommandListManager::DeleteGraphicsCommandList(std::size_t index)
{
	m_graphicsCommandLists.erase(m_graphicsCommandLists.begin() + index);
}
void CommandListManager::ResetGraphicsCommandList(std::size_t index)
{
	auto commandList = m_graphicsCommandLists.at(index);
	auto commandAllocator = m_deviceResources->GetCommandAllocator();

	DX::ThrowIfFailed(commandList->Reset(commandAllocator, nullptr));
}

ID3D12GraphicsCommandList* CommandListManager::GetGraphicsCommandList(size_t index)
{
	return m_graphicsCommandLists[index].Get();
}

void CommandListManager::AddToExecutionQueue(size_t index)
{
	DX::ThrowIfFailed(m_graphicsCommandLists[index]->Close());

	m_executionQueue.push(index);
}

void CommandListManager::ExecuteCommandLists()
{
	std::vector<ID3D12CommandList*> commandLists(m_executionQueue.size());

	while(!m_executionQueue.empty())
	{
		auto index = m_executionQueue.front();

		commandLists.push_back(m_graphicsCommandLists[index].Get());

		m_executionQueue.pop();
	}

	m_deviceResources->GetCommandQueue()->ExecuteCommandLists(static_cast<UINT>(commandLists.size()), commandLists.data());
}

void CommandListManager::ExecuteCommandList(size_t index)
{
	std::array<ID3D12CommandList*, 1> ppCommandList = { m_graphicsCommandLists[index].Get() };
	m_deviceResources->GetCommandQueue()->ExecuteCommandLists(static_cast<UINT>(ppCommandList.size()), ppCommandList.data());
}
