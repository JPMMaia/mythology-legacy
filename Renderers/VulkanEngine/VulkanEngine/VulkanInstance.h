#pragma once

namespace VulkanEngine
{
	class VulkanInstance
	{
	public:
		VulkanInstance() = default;
		explicit VulkanInstance(const std::vector<const char*>& enabledExtensions);

	public:
		operator const vk::Instance&() const;

	private:
		static vk::UniqueInstance CreateInstance(const std::vector<const char*>& enabledExtensions);
		static bool CheckValidationLayerSupport(const std::vector<const char*>& validationLayers);

	private:
		vk::UniqueInstance m_instance;
	};
}
