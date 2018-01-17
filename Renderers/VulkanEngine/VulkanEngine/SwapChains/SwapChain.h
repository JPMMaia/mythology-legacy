#pragma once

#include "VulkanEngine/SwapChains/SwapChainSupportDetails.h"
#include "VulkanEngine/Surfaces/Surface.h"
#include "VulkanEngine/Devices/QueueFamilyIndices.h"
#include "VulkanEngine/Devices/DeviceManager.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace VulkanEngine
{
	class SwapChain
	{
	public:
		SwapChain(VkDevice device, const Surface& surface, const DeviceManager& deviceManager, VkRenderPass renderPass);
		~SwapChain();

	private:
		static VkSwapchainKHR CreateSwapChain(VkDevice device, VkSurfaceKHR surface, const DeviceManager& deviceManager);
		static std::vector<VkImage> CreateImages(VkDevice device, VkSwapchainKHR swapChain);
		static std::vector<VkImageView> CreateImageViews(VkDevice device, const std::vector<VkImage>& images, VkFormat imageFormat);
		static std::vector<VkFramebuffer> CreateFrameBuffers(VkDevice device, const std::vector<VkImageView>& imageViews, std::uint32_t width, std::uint32_t height, VkRenderPass renderPass);

	private:
		VkDevice m_device;
		VkSwapchainKHR m_swapChain;
		std::vector<VkImage> m_images;
		std::vector<VkImageView> m_imageViews;
		std::vector<VkFramebuffer> m_framebuffers;
	};
}
