#include "pch.h"
#include "SwapChain.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"

#include <algorithm>
#include <array>

using namespace VulkanEngine;

SwapChain::SwapChain(VkDevice device, const Surface& surface, const DeviceManager& deviceManager, VkRenderPass renderPass) :
	m_device(device),
	m_swapChain(CreateSwapChain(device, surface, deviceManager)),
	m_images(CreateImages(device, m_swapChain)),
	m_imageViews(CreateImageViews(device, m_images, deviceManager.GetSwapSurfaceFormat().format)),
	m_framebuffers(CreateFrameBuffers(device, m_imageViews, surface.GetWidth(), surface.GetHeight(), renderPass))
{
}
SwapChain::~SwapChain()
{
	for (auto framebuffer : m_framebuffers)
		vkDestroyFramebuffer(m_device, framebuffer, nullptr);

	for (auto imageView : m_imageViews)
		vkDestroyImageView(m_device, imageView, nullptr);

	if (m_swapChain)
		vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
}

VkFramebuffer SwapChain::GetFrameBuffer(std::size_t index) const
{
	return m_framebuffers[index];
}

SwapChain::operator VkSwapchainKHR() const
{
	return m_swapChain;
}

VkSwapchainKHR SwapChain::CreateSwapChain(VkDevice device, VkSurfaceKHR surface, const DeviceManager& deviceManager)
{
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = deviceManager.GetImageCount();

	auto surfaceFormat = deviceManager.GetSwapSurfaceFormat();
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;

	createInfo.imageExtent = deviceManager.GetSwapExtent();
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	auto indices = deviceManager.GetQueueFamilyIndices();
	std::array<std::uint32_t, 2> queueFamilyIndices = 
	{ 
		static_cast<std::uint32_t>(indices.GraphicsFamily), 
		static_cast<std::uint32_t>(indices.PresentFamily) 
	};

	if (indices.GraphicsFamily != indices.PresentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = static_cast<std::uint32_t>(queueFamilyIndices.size());
		createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	createInfo.preTransform = deviceManager.GetPreTransform();
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = deviceManager.GetSwapPresentMode();
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	VkSwapchainKHR swapChain;
	ThrowIfFailed(vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain));
	return swapChain;
}
std::vector<VkImage> SwapChain::CreateImages(VkDevice device, VkSwapchainKHR swapChain)
{
	std::uint32_t imageCount;
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);

	std::vector<VkImage> images(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, images.data());
	return images;
}
std::vector<VkImageView> SwapChain::CreateImageViews(VkDevice device, const std::vector<VkImage>& images, VkFormat imageFormat)
{
	std::vector<VkImageView> imageViews(images.size());
	for (std::size_t i = 0; i < imageViews.size(); i++)
	{
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = images[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = imageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		ThrowIfFailed(vkCreateImageView(device, &createInfo, nullptr, &imageViews[i]));
	}

	return imageViews;
}
std::vector<VkFramebuffer> SwapChain::CreateFrameBuffers(VkDevice device, const std::vector<VkImageView>& imageViews, std::uint32_t width, std::uint32_t height, VkRenderPass renderPass)
{
	std::vector<VkFramebuffer> frameBuffers(imageViews.size());

	for (size_t i = 0; i < frameBuffers.size(); i++)
	{
		VkImageView attachments[] = 
		{
			imageViews[i]
		};

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = width;
		framebufferInfo.height = height;
		framebufferInfo.layers = 1;

		ThrowIfFailed(vkCreateFramebuffer(device, &framebufferInfo, nullptr, &frameBuffers[i]));
	}

	return frameBuffers;
}
