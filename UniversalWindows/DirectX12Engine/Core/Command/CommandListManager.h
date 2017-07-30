#pragma once
#include "Core/DeviceResources.h"
#include <queue>

namespace DirectX12Engine
{
	class CommandListManager
	{
	public:
		explicit CommandListManager(const std::shared_ptr<DeviceResources>& deviceResources);

		std::size_t CreateGraphicsCommandList(ID3D12GraphicsCommandList*& graphicsCommandList);
		void DeleteGraphicsCommandList(std::size_t index);
		void ResetGraphicsCommandList(std::size_t index);

		ID3D12GraphicsCommandList* GetGraphicsCommandList(std::size_t index);

		void AddToExecutionQueue(std::size_t index);
		void ExecuteCommandLists();
		void ExecuteCommandList(std::size_t index);

	private:
		std::shared_ptr<DeviceResources> m_deviceResources;

		std::vector<Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>> m_graphicsCommandLists;
		std::queue<size_t> m_executionQueue;
	};
}
