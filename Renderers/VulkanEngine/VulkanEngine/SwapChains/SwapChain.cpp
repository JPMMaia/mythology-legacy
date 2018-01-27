#include "pch.h"
#include "SwapChain.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"

using namespace VulkanEngine;

SwapChain::SwapChain(vk::Device device, const Surface& surface, const SwapChainSupportDetails& supportDetails, const QueueFamilyIndices& queueFamilyIndices, const vk::RenderPass& renderPass, vk::SwapchainKHR oldSwapChain) :
	m_extent(ChooseExtent(supportDetails, surface.GetExtent())),
	m_surfaceFormat(ChooseSurfaceFormat(supportDetails)),
	m_presentMode(ChoosePresentMode(supportDetails)),
	m_imageCount(ChooseImageCount(supportDetails)),
	m_swapChain(CreateSwapChain(device, surface, queueFamilyIndices, m_extent, m_surfaceFormat, m_presentMode, m_imageCount, supportDetails.Capabilities.currentTransform, oldSwapChain)),
	m_images(CreateImages(device, m_swapChain.get())),
	m_imageViews(CreateImageViews(device, m_images, m_surfaceFormat.format)),
	m_framebuffers(CreateFrameBuffers(device, m_imageViews, m_extent, renderPass))
{
}

const vk::Extent2D& SwapChain::GetExtent() const
{
	return m_extent;
}
const vk::SurfaceFormatKHR& SwapChain::GetSurfaceFormat() const
{
	return m_surfaceFormat;
}
vk::PresentModeKHR SwapChain::GetPresentMode() const
{
	return m_presentMode;
}
std::uint32_t SwapChain::GetImageCount() const
{
	return m_imageCount;
}
const vk::Framebuffer& SwapChain::GetFrameBuffer(std::size_t index) const
{
	return m_framebuffers[index].get();
}

SwapChain::operator const vk::SwapchainKHR&() const
{
	return m_swapChain.get();
}

vk::Extent2D SwapChain::ChooseExtent(const SwapChainSupportDetails& supportDetails, vk::Extent2D surfaceExtent)
{
	const auto& capabilities = supportDetails.Capabilities;
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		return surfaceExtent;

	vk::Extent2D actualExtent = surfaceExtent;
	actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
	actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
	return actualExtent;
}
vk::SurfaceFormatKHR SwapChain::ChooseSurfaceFormat(const SwapChainSupportDetails& supportDetails)
{
	const auto& availableFormats = supportDetails.Formats;

	if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined)
		return { vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear };

	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == vk::Format::eB8G8R8A8Unorm && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
			return availableFormat;
	}

	return availableFormats[0];
}
vk::PresentModeKHR SwapChain::ChoosePresentMode(const SwapChainSupportDetails& supportDetails)
{
	const auto& availablePresentModes = supportDetails.PresentModes;

	auto bestMode = vk::PresentModeKHR::eFifo;

	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == vk::PresentModeKHR::eMailbox)
			return availablePresentMode;
		else if (availablePresentMode == vk::PresentModeKHR::eImmediate)
			bestMode = availablePresentMode;
	}

	return bestMode;
}
std::uint32_t SwapChain::ChooseImageCount(const SwapChainSupportDetails& supportDetails)
{
	const auto& capabilities = supportDetails.Capabilities;

	auto imageCount = capabilities.minImageCount + 1;
	if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
		imageCount = capabilities.maxImageCount;

	return imageCount;
}

vk::UniqueSwapchainKHR SwapChain::CreateSwapChain(const vk::Device& device, const vk::SurfaceKHR& surface, const QueueFamilyIndices& queueFamilyIndices, const vk::Extent2D& extent, const vk::SurfaceFormatKHR& surfaceFormat, vk::PresentModeKHR presentMode, std::uint32_t imageCount, vk::SurfaceTransformFlagBitsKHR preTransform, vk::SwapchainKHR oldSwapChain)
{
	std::array<std::uint32_t, 2> indices =
	{
		static_cast<std::uint32_t>(queueFamilyIndices.GraphicsFamily),
		static_cast<std::uint32_t>(queueFamilyIndices.PresentFamily)
	};

	vk::SwapchainCreateInfoKHR createInfo(
		{},
		surface,
		imageCount,
		surfaceFormat.format,
		surfaceFormat.colorSpace,
		extent,
		1,
		vk::ImageUsageFlagBits::eColorAttachment,
		indices[0] != indices[1] ? vk::SharingMode::eConcurrent : vk::SharingMode::eExclusive,
		indices[0] != indices[1] ? static_cast<std::uint32_t>(indices.size()) : 0,
		indices[0] != indices[1] ? indices.data() : nullptr,
		preTransform,
		vk::CompositeAlphaFlagBitsKHR::eOpaque,
		presentMode,
		VK_TRUE,
		oldSwapChain
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
std::vector<vk::UniqueFramebuffer> SwapChain::CreateFrameBuffers(const vk::Device& device, const std::vector<vk::UniqueImageView>& imageViews, const vk::Extent2D& extent, const vk::RenderPass& renderPass)
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
			extent.width, extent.height,
			1
		);

		frameBuffers[i] = device.createFramebufferUnique(framebufferInfo);
	}

	return frameBuffers;
}
