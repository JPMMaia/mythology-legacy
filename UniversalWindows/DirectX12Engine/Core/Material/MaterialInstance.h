#pragma once

#include "Core/Shader/ShaderBufferTypes.h"

namespace DirectX12Engine
{
	/*class MaterialInstance
	{
	public:
		using MaterialType = ShaderBufferTypes::MaterialData;
		using FrameResourceType = FrameResource<MaterialInstance, MaterialType>;

	private:
		friend class FrameResourceType;

	public:
		MaterialInstance(size_t id, const MaterialType& data, FrameResourceType& frameResource) :
			m_id(id),
			m_data(data),
			m_frameResource(frameResource)
		{
		}

		void Update(const MaterialType& data) const
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
		MaterialType m_data;
		size_t m_dirtyFrames = 0;
		FrameResourceType& m_frameResource;
	};*/
}
