#pragma once

namespace GraphicsEngine
{
	template<typename ResourceDataType>
	class Resource
	{
		friend class FrameResource;

	public:
		
	private:
		uint32_t m_index = 0;
		ResourceDataType m_data;
	};
}
