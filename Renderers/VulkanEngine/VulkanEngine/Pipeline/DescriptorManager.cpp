#include "pch.h"
#include "DescriptorManager.h"

using namespace VulkanEngine;

DescriptorManager::DescriptorManager(vk::Device device) :
	m_device(device),
	m_descriptorPool(CreateDescriptorPool(device)),
	m_descriptorSetLayouts(CreateDescriptorSetLayouts(device)),
	m_descriptorSets(CreateDescriptorSets(device, *m_descriptorPool, m_descriptorSetLayouts))
{
}
DescriptorManager::~DescriptorManager()
{
	for (auto descriptorSetLayout : m_descriptorSetLayouts)
		m_device.destroyDescriptorSetLayout(descriptorSetLayout);
}

const std::vector<vk::DescriptorSetLayout>& DescriptorManager::GetDescriptorSetLayouts() const
{
	return m_descriptorSetLayouts;
}
const std::vector<vk::DescriptorSet>& DescriptorManager::GetDescriptorSets() const
{
	return m_descriptorSets;
}

vk::UniqueDescriptorPool DescriptorManager::CreateDescriptorPool(vk::Device device)
{
	std::array<vk::DescriptorPoolSize, 1> descriptorPoolSizes =
	{
		vk::DescriptorPoolSize(vk::DescriptorType::eUniformBuffer, 2)
	};

	vk::DescriptorPoolCreateInfo createInfo(
		{},
		2,
		static_cast<std::uint32_t>(descriptorPoolSizes.size()), descriptorPoolSizes.data()
	);
	return device.createDescriptorPoolUnique(createInfo);
}
std::vector<vk::DescriptorSetLayout> DescriptorManager::CreateDescriptorSetLayouts(vk::Device device)
{
	std::array<vk::DescriptorSetLayoutBinding, 1> bindings =
	{
		vk::DescriptorSetLayoutBinding(0, vk::DescriptorType::eUniformBuffer, 1, vk::ShaderStageFlagBits::eVertex),
	};

	vk::DescriptorSetLayoutCreateInfo createInfo(
		{},
		static_cast<std::uint32_t>(bindings.size()), bindings.data()
	);
	return
	{
		device.createDescriptorSetLayout(createInfo),
		device.createDescriptorSetLayout(createInfo),
	};
}
std::vector<vk::DescriptorSet> DescriptorManager::CreateDescriptorSets(vk::Device device, vk::DescriptorPool descriptorPool, const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts)
{
	vk::DescriptorSetAllocateInfo allocateInfo(
		descriptorPool,
		static_cast<std::uint32_t>(descriptorSetLayouts.size()), descriptorSetLayouts.data()
	);
	return device.allocateDescriptorSets(allocateInfo);
}
