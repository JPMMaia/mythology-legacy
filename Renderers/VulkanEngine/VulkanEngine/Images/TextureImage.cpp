#include "pch.h"
#include "TextureImage.h"

using namespace VulkanEngine;

TextureImage::TextureImage(const DeviceManager& deviceManager, vk::DeviceSize size, vk::Format format, vk::Extent2D extent, vk::ImageUsageFlags usage) :
	m_image(CreateImage(deviceManager.GetDevice(), size, format, extent, usage)),
	m_deviceMemory(AllocateDeviceMemory(deviceManager, m_image.get(), vk::MemoryPropertyFlagBits::eDeviceLocal))
{
	BindImageMemory(deviceManager.GetDevice());
}

vk::Image TextureImage::operator*()
{
	return *m_image;
}

vk::UniqueImage TextureImage::CreateImage(vk::Device device, vk::DeviceSize size, vk::Format format, vk::Extent2D extent, vk::ImageUsageFlags usage)
{
	vk::ImageCreateInfo createInfo(
		{},
		vk::ImageType::e2D,
		format,
		vk::Extent3D(extent.width, extent.height, 1),
		1,
		1,
		vk::SampleCountFlagBits::e1,
		vk::ImageTiling::eOptimal,
		usage,
		vk::SharingMode::eExclusive,
		0, nullptr,
		vk::ImageLayout::eUndefined
	);

	return device.createImageUnique(createInfo);
}
vk::UniqueDeviceMemory TextureImage::AllocateDeviceMemory(const DeviceManager& deviceManager, vk::Image image, vk::MemoryPropertyFlags memoryProperties)
{
	auto device = deviceManager.GetDevice();

	auto memoryRequirements = device.getImageMemoryRequirements(image);
	auto memoryTypeIndex = deviceManager.FindBufferMemoryIndexType(
		memoryRequirements.memoryTypeBits,
		memoryProperties
	);

	vk::MemoryAllocateInfo allocateInfo(
		memoryRequirements.size,
		memoryTypeIndex
	);

	return device.allocateMemoryUnique(allocateInfo);
}

void TextureImage::BindImageMemory(const vk::Device& device)
{
	device.bindImageMemory(m_image.get(), m_deviceMemory.get(), 0);
}
