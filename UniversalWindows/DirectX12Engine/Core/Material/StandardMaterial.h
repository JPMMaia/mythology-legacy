#pragma once

#include "GraphicsEngineInterfaces/IMaterial.h"
#include "Core/Utilities/UploadBuffer.h"
#include "Core/Shader/ShaderBufferTypes.h"
#include "Core/Resources/FrameResource.h"
#include "MaterialInstance.h"

namespace DirectX12Engine
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
