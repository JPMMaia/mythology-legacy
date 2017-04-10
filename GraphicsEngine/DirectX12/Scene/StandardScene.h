#pragma once

#include "Interfaces/IScene.h"
#include "Interfaces/IRenderItem.h"
#include "DirectX12/DeviceResources.h"
#include <unordered_map>
#include "Interfaces/IMaterial.h"
#include "Interfaces/IMesh.h"

namespace GraphicsEngine
{
	class StandardScene : public IScene
	{
	public:
		explicit StandardScene(const std::shared_ptr<DeviceResources>& deviceResources);
		~StandardScene();

	public:
		void CreateDeviceDependentResources() override;
		void CreateWindowSizeDependentResources() override;
		
		void SaveState() override;
		void LoadState() override;

		void FrameUpdate(const Common::Timer& timer) override;
		void FixedUpdate(const Common::Timer& timer) override;

	private:
		std::shared_ptr<DeviceResources> m_deviceResources;

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;

		std::unordered_map<std::string, std::shared_ptr<IMesh>> m_meshes;
		std::unordered_map<std::string, std::shared_ptr<IMaterial>> m_materials;
		std::unordered_map<std::string, std::shared_ptr<IRenderItem>> m_renderItems;
	};
}
