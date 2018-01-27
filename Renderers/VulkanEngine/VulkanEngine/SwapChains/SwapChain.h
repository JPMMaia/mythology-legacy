#pragma once

#include "VulkanEngine/SwapChains/SwapChainSupportDetails.h"
#include "VulkanEngine/Surfaces/Surface.h"
#include "VulkanEngine/Devices/QueueFamilyIndices.h"
#include "VulkanEngine/Devices/DeviceManager.h"

namespace VulkanEngine
{
	class SwapChain
	{
	public:
		SwapChain(vk::Device device, const Surface& surface, const SwapChainSupportDetails& supportDetails, const QueueFamilyIndices& queueFamilyIndices, const vk::RenderPass& renderPass, vk::SwapchainKHR oldSwapChain = {});

	public:
		const vk::Extent2D& GetExtent() const;
		const vk::SurfaceFormatKHR& GetSurfaceFormat() const;
		vk::PresentModeKHR GetPresentMode() const;
		std::uint32_t GetImageCount() const;
		const vk::Framebuffer& GetFrameBuffer(std::size_t index) const;

	public:
		operator const vk::SwapchainKHR&() const;

	private:
		static vk::Extent2D ChooseExtent(const SwapChainSupportDetails& supportDetails, vk::Extent2D surfaceExtent);
		static vk::SurfaceFormatKHR ChooseSurfaceFormat(const SwapChainSupportDetails& supportDetails);
		static vk::PresentModeKHR ChoosePresentMode(const SwapChainSupportDetails& supportDetails);
		static std::uint32_t ChooseImageCount(const SwapChainSupportDetails& supportDetails);
		static vk::UniqueSwapchainKHR CreateSwapChain(const vk::Device& device, const vk::SurfaceKHR& surface, const QueueFamilyIndices& queueFamilyIndices, const vk::Extent2D& extent, const vk::SurfaceFormatKHR& surfaceFormat, vk::PresentModeKHR presentMode, std::uint32_t imageCount, vk::SurfaceTransformFlagBitsKHR preTransform, vk::SwapchainKHR oldSwapChain);
		static std::vector<vk::Image> CreateImages(const vk::Device& device, const vk::SwapchainKHR& swapChain);
		static std::vector<vk::UniqueImageView> CreateImageViews(const vk::Device& device, const std::vector<vk::Image>& images, vk::Format imageFormat);
		static std::vector<vk::UniqueFramebuffer> CreateFrameBuffers(const vk::Device& device, const std::vector<vk::UniqueImageView>& imageViews, const vk::Extent2D& extent, const vk::RenderPass& renderPass);

	private:
		vk::Extent2D m_extent;
		vk::SurfaceFormatKHR m_surfaceFormat;
		vk::PresentModeKHR m_presentMode;
		std::uint32_t m_imageCount;
		vk::UniqueSwapchainKHR m_swapChain;
		std::vector<vk::Image> m_images;
		std::vector<vk::UniqueImageView> m_imageViews;
		std::vector<vk::UniqueFramebuffer> m_framebuffers;
	};
}
