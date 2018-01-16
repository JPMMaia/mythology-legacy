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

using namespace std;
using namespace VulkanEngine;

PipelineStateManager::PipelineStateManager(VkDevice device, VkFormat format, float width, float height, VkExtent2D extent) :
	m_device(device),
	m_renderPass(device, format),
	m_pipelineLayout(device),
	m_shaders(CreateShaders(device)),
	m_graphicsPipeline(CreateGraphicsPipeline(device, m_renderPass, m_pipelineLayout, m_shaders, width, height, extent))
{
}
PipelineStateManager::~PipelineStateManager()
{
	if(m_graphicsPipeline)
		vkDestroyPipeline(m_device, m_graphicsPipeline, nullptr);
}

std::unordered_map<std::string, Shader> PipelineStateManager::CreateShaders(VkDevice device)
{
	vector<pair<string, wstring>> shadersInfo =
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

VkPipeline VulkanEngine::PipelineStateManager::CreateGraphicsPipeline(VkDevice device, const RenderPass& renderPass, const PipelineLayout& pipelineLayout, const ShaderContainer& shaders, float width, float height, VkExtent2D extent)
{
	std::vector<VkPipelineShaderStageCreateInfo> shaderStages =
	{
		ShaderStage::Vertex(shaders.at("StandardVertexShader").GetShaderModule(), "main"),
		ShaderStage::Fragment(shaders.at("StandardFragmentShader").GetShaderModule(), "main")
	};

	auto vertexInputState = VertexInputState::Default();
	auto inputAssembly = InputAssemblyState::Default();
	auto viewportState = ViewportState::Default(width, height, extent);
	auto rasterizer = RasterizerState::Default();
	auto multisampling = MultisampleState::Default();

	std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments =
	{
		ColorBlendAttachment::Default()
	};
	auto colorBlendState = ColorBlendState::Default(colorBlendAttachments);

	std::vector<VkDynamicState> dynamicStates =
	{
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};
	auto dynamicState = DynamicState::Default(dynamicStates);

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = static_cast<std::uint32_t>(shaderStages.size());
	pipelineInfo.pStages = shaderStages.data();
	pipelineInfo.pVertexInputState = &vertexInputState;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr;
	pipelineInfo.pColorBlendState = &colorBlendState;
	pipelineInfo.pDynamicState = nullptr;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;

	VkPipeline graphicsPipeline;
	ThrowIfFailed(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline));
	return graphicsPipeline;
}
