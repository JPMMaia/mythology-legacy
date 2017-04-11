#pragma once
#include "DirectX12/DeviceResources.h"
#include <queue>

namespace GraphicsEngine
{
	class CommandListManager
	{
	public:
		explicit CommandListManager(std::shared_ptr<DeviceResources>& deviceResources);

		size_t CreateGraphicsCommandList(ID3D12GraphicsCommandList*& graphicsCommandList);

		ID3D12GraphicsCommandList* GetGraphicsCommandList(size_t index);

		void AddToExecutionQueue(size_t index);
		void ExecuteCommandLists();
		void ExecuteCommandList(size_t index);

	private:
		std::shared_ptr<DeviceResources> m_deviceResources;

		std::vector<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>> m_graphicsCommandLists;
		std::queue<size_t> m_executionQueue;
	};
}
