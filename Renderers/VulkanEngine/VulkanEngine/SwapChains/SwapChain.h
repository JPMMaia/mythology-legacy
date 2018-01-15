#pragma once

#include "VulkanEngine/SwapChains/SwapChainSupportDetails.h"
#include "VulkanEngine/Surfaces/Surface.h"
#include "VulkanEngine/Devices/QueueFamilyIndices.h"

#include <vulkan/vulkan.h>
#include <vector>

namespace VulkanEngine
{
	class SwapChain
	{
	public:
		SwapChain(VkDevice device, const Surface& surface, const SwapChainSupportDetails& swapChainSupport, const QueueFamilyIndices& indices);
		~SwapChain();

	private:
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, std::uint32_t width, std::uint32_t height);

	private:
		void CreateImageViews();

	private:
		VkDevice m_device;
		VkSwapchainKHR m_swapChain;
		std::vector<VkImage> m_swapChainImages;
		VkFormat m_swapChainImageFormat;
		VkExtent2D m_swapChainExtent;
		std::vector<VkImageView> m_swapChainImageViews;
	};
}
