#include "pch.h"
#include "PipelineStateManager.h"
#include "VulkanEngine/Helpers/VulkanHelpers.h"

#include "States/ColorBlendAttachment.h"
#include "States/ColorBlendState.h"
#include "States/DynamicState.h"
#include "States/InputAssemblyState.h"
#include "States/MultisampleState.h"
#include "States/RasterizerState.h"
#include "States/ShaderStage.h"
#include "States/VertexInputState.h"
#include "States/ViewportState.h"

#include "VulkanEngine/Geometry/Vertex.h"

using namespace std;
using namespace VulkanEngine;

PipelineStateManager::PipelineStateManager(const vk::Device& device, vk::Format format, float width, float height, const vk::Extent2D& extent) :
	m_viewport(0.0f, 0.0f, width, height, 0.0f, 1.0f),
	m_scissor({ 0, 0 }, extent),
	m_renderPass(device, format),
	m_pipelineLayout(device),
	m_shaders(CreateShaders(device)),
	m_graphicsPipeline(CreateGraphicsPipeline(device, m_viewport, m_scissor, m_renderPass, m_pipelineLayout, m_shaders, width, height, extent))
{
}

const RenderPass& PipelineStateManager::GetRenderPass() const
{
	return m_renderPass;
}
const vk::Pipeline& PipelineStateManager::GetGraphicsPipeline() const
{
	return m_graphicsPipeline.get();
}

std::unordered_map<std::string, Shader> PipelineStateManager::CreateShaders(const vk::Device& device)
{
	std::vector<std::pair<std::string, std::wstring>> shadersInfo =
	{
		{ "StandardVertexShader", L"Shaders/vert.spv" },
		{ "StandardFragmentShader", L"Shaders/frag.spv" }
	};

	std::unordered_map<std::string, Shader> shaders;
	for (const auto& shaderInfo : shadersInfo)
	{
		shaders.emplace(
			std::piecewise_construct,
			std::forward_as_tuple(shaderInfo.first),
			std::forward_as_tuple(device, shaderInfo.second)
		);
	}

	return shaders;
}

vk::UniquePipeline PipelineStateManager::CreateGraphicsPipeline(const vk::Device& device, const vk::Viewport& viewport, const vk::Rect2D& scissor, const RenderPass& renderPass, const PipelineLayout& pipelineLayout, const ShaderContainer& shaders, float width, float height, const vk::Extent2D& extent)
{
	std::vector<vk::PipelineShaderStageCreateInfo> shaderStages =
	{
		ShaderStage::Vertex(shaders.at("StandardVertexShader"), "main"),
		ShaderStage::Fragment(shaders.at("StandardFragmentShader"), "main")
	};

	auto vertexBindingDescription = Vertex::GetBindingDescription();
	auto vertexAttributeDescriptions = Vertex::GetAttributeDescriptions();
	auto vertexInputState = VertexInputState::Default(vertexBindingDescription, vertexAttributeDescriptions);
	auto inputAssembly = InputAssemblyState::TriangleList();
	auto viewportState = ViewportState::Default(viewport, scissor);
	auto rasterizer = RasterizerState::Default();
	auto multisampling = MultisampleState::Default();

	std::vector<vk::PipelineColorBlendAttachmentState> colorBlendAttachments =
	{
		ColorBlendAttachment::Default()
	};
	auto colorBlendState = ColorBlendState::Default(colorBlendAttachments);

	std::vector<vk::DynamicState> dynamicStates =
	{
		vk::DynamicState::eViewport,
		vk::DynamicState::eLineWidth
	};
	auto dynamicState = DynamicState::Default(dynamicStates);

	vk::GraphicsPipelineCreateInfo pipelineInfo(
		{},
		static_cast<std::uint32_t>(shaderStages.size()), shaderStages.data(),
		&vertexInputState,
		&inputAssembly,
		nullptr,
		&viewportState,
		&rasterizer,
		&multisampling,
		nullptr,
		&colorBlendState,
		&dynamicState,
		pipelineLayout,
		renderPass,
		0
	);

	return device.createGraphicsPipelineUnique({}, pipelineInfo);
}
