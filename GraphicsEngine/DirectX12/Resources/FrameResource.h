#pragma once

#include "DirectX12/Utilities/UploadBuffer.h"
#include "DirectX12/DeviceResources.h"

#include <vector>
#include <queue>
#include <unordered_map>

namespace GraphicsEngine
{
	template<typename ResourceType, typename ResourceShaderDataType>
	class FrameResource
	{
	public:
		//using ResourceType = MaterialInstance;
		//using ResourceShaderDataType = ShaderBufferTypes::MaterialType;

	private:
		enum class ResourceChangeType
		{
			Add,
			Remove,
			Update
		};
		struct ResourceChange
		{
			size_t ID;
			ResourceChangeType Type;
		};

	public:
		explicit FrameResource(const std::shared_ptr<DeviceResources>& deviceResources, size_t initialCapacity, bool isConstantBuffer) :
			m_deviceResources(deviceResources),
			m_isConstantBuffer(isConstantBuffer),
			m_resourcesData(deviceResources->GetFrameCount())
		{
			auto d3dDevice = deviceResources->GetD3DDevice();

			for(size_t i = 0; i < m_resourcesData.size(); ++i)
			{
				m_resourcesData[i] = std::make_unique<UploadBuffer<ResourceShaderDataType>>(d3dDevice, initialCapacity, isConstantBuffer);
			}

			m_resourceChanges.resize(deviceResources->GetFrameCount());
		}

		std::shared_ptr<ResourceType> AddResource(ResourceShaderDataType&& resourceData)
		{
			auto id = m_count++;

			auto resource = std::make_shared<ResourceType>(id, resourceData, *this);
			resource->m_dirtyFrames = m_deviceResources->GetFrameCount();
			m_resources.emplace(id, resource);

			GetCurrentFrameChanges().push({ id, ResourceChangeType::Add });

			return resource;
		}
		void RemoveResource(size_t id)
		{
			m_resources.erase(id);

			GetCurrentFrameChanges().push({ id, ResourceChangeType::Remove });
		}
		void UpdateResource(size_t id, const ResourceShaderDataType& resourceData)
		{
			m_resources.at(id)->m_data = resourceData;

			GetCurrentFrameChanges().push({ id, ResourceChangeType::Update });
		}

		void FrameUpdate(ID3D12Device* d3dDevice)
		{
			auto currentFrameChanges = GetCurrentFrameChanges();
			auto nextFrameChanges = GetNextFrameChanges();
			while (!currentFrameChanges.empty())
			{
				const auto& change = currentFrameChanges.front();

				if (change.Type == ResourceChangeType::Add)
				{
					if (IsCurrentUploadBufferFull())
					{
						EnlargeUploadBuffer(d3dDevice);
					}

					auto shaderIndex = m_highestShaderIndex == 0 ? 0 : ++m_highestShaderIndex;

					auto& resource = m_resources[change.ID];
					resource->m_shaderIndex = shaderIndex;

					GetCurrentFrameData()->CopyData(static_cast<INT>(shaderIndex), resource->m_data);

					if(resource->m_dirtyFrames != 0)
					{
						if (--resource->m_dirtyFrames != 0)
							nextFrameChanges.push(change);
					}	
				}
				if (change.Type == ResourceChangeType::Remove)
				{
					if (!IsCurrentUploadBufferCompressed())
					{
						CompressUploadBuffer(d3dDevice);

						if (m_highestShaderIndex >= GetCurrentFrameData()->GetElementCount())
							ReassignIndices();
					}
				}
				if (change.Type == ResourceChangeType::Update)
				{
					auto& resource = m_resources[change.ID];

					GetCurrentFrameData()->CopyData(static_cast<INT>(resource->m_shaderIndex), resource->m_data);

					/*--resource->m_dirtyFrames;
					if (resource->m_dirtyFrames != 0)
						nextFrameChanges.push(change);*/
				}

				currentFrameChanges.pop();
			}
		}

		void BindBuffer(ID3D12GraphicsCommandList* commandList)
		{
			commandList->SetGraphicsRootShaderResourceView(2, GetCurrentFrameData()->GetGPUVirtualAddress(0));
		}

	private:		
		void EnlargeUploadBuffer(ID3D12Device* d3dDevice)
		{
			auto desiredSize = static_cast<size_t>(std::powf(2.0f, std::ceilf(std::sqrtf(static_cast<float>(m_resources.size())) + 1.0f)));

			m_resourcesData[m_deviceResources->GetCurrentFrameIndex()] = std::make_unique<UploadBuffer<ResourceShaderDataType>>(d3dDevice, desiredSize, m_isConstantBuffer);
		}
		void CompressUploadBuffer(ID3D12Device* d3dDevice)
		{
			auto desiredSize = static_cast<size_t>(std::powf(2.0f, std::ceilf(std::sqrtf(static_cast<float>(m_resources.size())))));

			m_resourcesData[m_deviceResources->GetCurrentFrameIndex()] = std::make_unique<UploadBuffer<ResourceShaderDataType>>(d3dDevice, desiredSize, m_isConstantBuffer);
		}
		void ReassignIndices()
		{
			size_t shaderIndex = 0;
			for(const auto& pair : m_resources)
			{
				pair.second->m_shaderIndex = shaderIndex++;
			}

			if(shaderIndex != 0)
				m_highestShaderIndex = shaderIndex - 1;
		}

		bool IsCurrentUploadBufferFull()
		{
			const auto& currentResourcesFrameData = m_resourcesData[m_deviceResources->GetCurrentFrameIndex()].get();
			if (currentResourcesFrameData->GetElementCount() <= m_highestShaderIndex)
				return true;

			return false;
		}
		bool IsCurrentUploadBufferCompressed()
		{
			auto desiredSize = static_cast<size_t>(std::powf(2.0f, std::ceilf(std::sqrtf(static_cast<float>(m_resources.size())))));

			const auto& currentUploadBuffer = m_resourcesData[m_deviceResources->GetCurrentFrameIndex()].get();
			if (desiredSize != currentUploadBuffer->GetElementCount())
				return false;

			return true;
		}

		std::queue<ResourceChange>& GetCurrentFrameChanges()
		{
			return m_resourceChanges.at(m_deviceResources->GetCurrentFrameIndex());
		}
		std::queue<ResourceChange>& GetNextFrameChanges()
		{
			return m_resourceChanges.at((m_deviceResources->GetCurrentFrameIndex() + 1) % m_deviceResources->GetFrameCount());
		}

		const std::unique_ptr<UploadBuffer<ResourceShaderDataType>>& GetCurrentFrameData()
		{
			return m_resourcesData.at(m_deviceResources->GetCurrentFrameIndex());
		}

	private:
		std::shared_ptr<DeviceResources> m_deviceResources;

		bool m_isConstantBuffer;
		std::unordered_map<size_t, std::shared_ptr<ResourceType>> m_resources;
		
		std::vector<std::unique_ptr<UploadBuffer<ResourceShaderDataType>>> m_resourcesData;
		size_t m_highestShaderIndex = 0;
		size_t m_count = 0;

		std::vector<std::queue<ResourceChange>> m_resourceChanges;
	};
}
