#pragma once

#include "Core/Resources/FrameResource.h"

namespace DirectX12Engine
{
	template<typename ShaderResourceType>
	class ResourceInstance
	{
	public:
		using FrameResourceType = FrameResource<ResourceInstance, ShaderResourceType>;

	private:
		friend class FrameResourceType;

	public:
		ResourceInstance(size_t id, const ShaderResourceType& data, FrameResourceType& frameResource) :
			m_id(id),
			m_data(data),
			m_frameResource(frameResource)
		{
		}

		void Update(const ShaderResourceType& data) const
		{
			m_frameResource.UpdateResource(m_id, data);
		}

		size_t GetID() const
		{
			return m_id;
		}
		size_t GetShaderIndex() const
		{
			return m_shaderIndex;
		}

	private:
		size_t m_id;
		size_t m_shaderIndex = 0;
		ShaderResourceType m_data;
		size_t m_dirtyFrames = 0;
		FrameResourceType& m_frameResource;
	};
}
