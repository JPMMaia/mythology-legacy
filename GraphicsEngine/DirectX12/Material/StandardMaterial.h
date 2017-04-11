#pragma once

#include "Interfaces/IMaterial.h"
#include "DirectX12/Utilities/UploadBuffer.h"
#include "DirectX12/Shader/ShaderBufferTypes.h"
#include "DirectX12/Resources/FrameResource.h"
#include "MaterialInstance.h"

namespace GraphicsEngine
{
	class StandardMaterial : public IMaterial
	{
	/*public:
		explicit StandardMaterial(const std::shared_ptr<DeviceResources>& deviceResources, size_t initialCapacity, bool isConstantBuffer) : 
		m_materialsData(deviceResources, initialCapacity, isConstantBuffer)
		{
		}

		std::shared_ptr<MaterialInstance> AddInstance(const DirectX::XMFLOAT4& baseColor);

		void BindMaterialBuffer(ID3D12GraphicsCommandList* commandList);

	private:
		MaterialInstance::FrameResource m_materialsData;*/
	};
}
