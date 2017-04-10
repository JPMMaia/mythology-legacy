#pragma once
#include <unordered_map>
#include "DirectX12/DeviceResources.h"

namespace GraphicsEngine
{
	class RootSignatureManager
	{
	public:
		explicit RootSignatureManager(const std::shared_ptr<DeviceResources>& deviceResources);

		void CreateDeviceDependentResources();

		ID3D12RootSignature* GetRootSignature(const std::string& name) const;
				
	private:
		std::shared_ptr<DeviceResources> m_deviceResources;

		std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D12RootSignature>> m_rootSignatures;
	};
}
