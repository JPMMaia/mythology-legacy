#pragma once

namespace VulkanEngine
{
	class DescriptorManager
	{
	public:
		explicit DescriptorManager(vk::Device device);
		~DescriptorManager();

	public:
		const std::vector<vk::DescriptorSetLayout>& GetDescriptorSetLayouts() const;
		const std::vector<vk::DescriptorSet>& GetDescriptorSets() const;

	private:
		static vk::UniqueDescriptorPool CreateDescriptorPool(vk::Device device);
		static std::vector<vk::DescriptorSetLayout> CreateDescriptorSetLayouts(vk::Device device);
		static std::vector<vk::DescriptorSet> CreateDescriptorSets(vk::Device device, vk::DescriptorPool descriptorPool, const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts);

	private:
		vk::Device m_device;
		vk::UniqueDescriptorPool m_descriptorPool;
		std::vector<vk::DescriptorSetLayout> m_descriptorSetLayouts;
		std::vector<vk::DescriptorSet> m_descriptorSets;
	};
}
