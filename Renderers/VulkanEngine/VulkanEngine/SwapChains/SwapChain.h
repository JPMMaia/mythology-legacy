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
		SwapChain(const vk::Device& device, const Surface& surface, const DeviceManager& deviceManager, const vk::RenderPass& renderPass);

	public:
		const vk::Framebuffer& GetFrameBuffer(std::size_t index) const;

	public:
		operator const vk::SwapchainKHR&() const;

	private:
		static vk::UniqueSwapchainKHR CreateSwapChain(const vk::Device& device, const vk::SurfaceKHR& surface, const DeviceManager& deviceManager);
		static std::vector<vk::Image> CreateImages(const vk::Device& device, const vk::SwapchainKHR& swapChain);
		static std::vector<vk::UniqueImageView> CreateImageViews(const vk::Device& device, const std::vector<vk::Image>& images, vk::Format imageFormat);
		static std::vector<vk::UniqueFramebuffer> CreateFrameBuffers(const vk::Device& device, const std::vector<vk::UniqueImageView>& imageViews, std::uint32_t width, std::uint32_t height, const vk::RenderPass& renderPass);

	private:
		vk::UniqueSwapchainKHR m_swapChain;
		std::vector<vk::Image> m_images;
		std::vector<vk::UniqueImageView> m_imageViews;
		std::vector<vk::UniqueFramebuffer> m_framebuffers;
	};
}
