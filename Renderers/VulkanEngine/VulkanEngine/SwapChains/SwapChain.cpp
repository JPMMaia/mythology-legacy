#include "pch.h"
#include "SwapChain.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"

using namespace VulkanEngine;

SwapChain::SwapChain(const vk::Device& device, const Surface& surface, const DeviceManager& deviceManager, const vk::RenderPass& renderPass) :
	m_swapChain(CreateSwapChain(device, surface, deviceManager)),
	m_images(CreateImages(device, m_swapChain.get())),
	m_imageViews(CreateImageViews(device, m_images, deviceManager.GetSwapSurfaceFormat().format)),
	m_framebuffers(CreateFrameBuffers(device, m_imageViews, surface.GetWidth(), surface.GetHeight(), renderPass))
{
}

const vk::Framebuffer& SwapChain::GetFrameBuffer(std::size_t index) const
{
	return m_framebuffers[index].get();
}

SwapChain::operator const vk::SwapchainKHR&() const
{
	return m_swapChain.get();
}

vk::UniqueSwapchainKHR SwapChain::CreateSwapChain(const vk::Device& device, const vk::SurfaceKHR& surface, const DeviceManager& deviceManager)
{
	auto surfaceFormat = deviceManager.GetSwapSurfaceFormat();

	auto indices = deviceManager.GetQueueFamilyIndices();
	std::array<std::uint32_t, 2> queueFamilyIndices =
	{
		static_cast<std::uint32_t>(indices.GraphicsFamily),
		static_cast<std::uint32_t>(indices.PresentFamily)
	};

	vk::SwapchainCreateInfoKHR createInfo(
		{},
		surface,
		deviceManager.GetImageCount(),
		surfaceFormat.format,
		surfaceFormat.colorSpace,
		deviceManager.GetSwapExtent(),
		1,
		vk::ImageUsageFlagBits::eColorAttachment,
		indices.GraphicsFamily != indices.PresentFamily ? vk::SharingMode::eConcurrent : vk::SharingMode::eExclusive,
		indices.GraphicsFamily != indices.PresentFamily ? static_cast<std::uint32_t>(queueFamilyIndices.size()) : 0,
		indices.GraphicsFamily != indices.PresentFamily ? queueFamilyIndices.data() : nullptr,
		deviceManager.GetPreTransform(),
		vk::CompositeAlphaFlagBitsKHR::eOpaque,
		deviceManager.GetSwapPresentMode(),
		VK_TRUE,
		{}
	);
	
	return device.createSwapchainKHRUnique(createInfo);
}
std::vector<vk::Image> SwapChain::CreateImages(const vk::Device& device, const vk::SwapchainKHR& swapChain)
{
	return device.getSwapchainImagesKHR(swapChain);
}
std::vector<vk::UniqueImageView> SwapChain::CreateImageViews(const vk::Device& device, const std::vector<vk::Image>& images, vk::Format imageFormat)
{
	std::vector<vk::UniqueImageView> imageViews(images.size());
	for (std::size_t i = 0; i < imageViews.size(); i++)
	{
		vk::ImageViewCreateInfo createInfo(
			{},
			images[i],
			vk::ImageViewType::e2D,
			imageFormat,
			vk::ComponentMapping(),
			vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1)
		);

		imageViews[i] = device.createImageViewUnique(createInfo);
	}

	return imageViews;
}
std::vector<vk::UniqueFramebuffer> SwapChain::CreateFrameBuffers(const vk::Device& device, const std::vector<vk::UniqueImageView>& imageViews, std::uint32_t width, std::uint32_t height, const vk::RenderPass& renderPass)
{
	std::vector<vk::UniqueFramebuffer> frameBuffers(imageViews.size());

	for (std::size_t i = 0; i < frameBuffers.size(); i++)
	{
		std::array<vk::ImageView, 1> attachments = 
		{
			imageViews[i].get()
		};

		vk::FramebufferCreateInfo framebufferInfo(
			{},
			renderPass,
			static_cast<std::uint32_t>(attachments.size()), attachments.data(),
			width, height,
			1
		);

		frameBuffers[i] = device.createFramebufferUnique(framebufferInfo);
	}

	return frameBuffers;
}
