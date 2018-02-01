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

PipelineStateManager::PipelineStateManager(vk::Device device, const DescriptorManager& descriptorManager, const RenderPass& renderPass) :
	m_pipelineLayout(CreatePipelineLayout(device, descriptorManager.GetDescriptorSetLayouts())),
	m_graphicsPipeline(CreateGraphicsPipeline(device, renderPass, m_pipelineLayout.get()))
{
}

vk::PipelineLayout PipelineStateManager::GetPipelineLayout() const
{
	return m_pipelineLayout.get();
}

vk::Pipeline PipelineStateManager::GetGraphicsPipeline() const
{
	return m_graphicsPipeline.get();
}

vk::UniquePipelineLayout PipelineStateManager::CreatePipelineLayout(vk::Device device, const std::vector<vk::DescriptorSetLayout>& descriptorSetLayouts)
{
	vk::PipelineLayoutCreateInfo createInfo(
		{},
		static_cast<std::uint32_t>(descriptorSetLayouts.size()), descriptorSetLayouts.data()
	);
	return device.createPipelineLayoutUnique(createInfo);
}
vk::UniquePipeline PipelineStateManager::CreateGraphicsPipeline(vk::Device device, const RenderPass& renderPass, vk::PipelineLayout pipelineLayout)
{
	auto shaders = CreateShaders(device);
	std::vector<vk::PipelineShaderStageCreateInfo> shaderStages =
	{
		ShaderStage::Vertex(shaders.at("StandardVertexShader"), "main"),
		ShaderStage::Fragment(shaders.at("StandardFragmentShader"), "main")
	};

	auto vertexBindingDescription = Vertex::GetBindingDescription();
	auto vertexAttributeDescriptions = Vertex::GetAttributeDescriptions();
	auto vertexInputState = VertexInputState::Default(vertexBindingDescription, vertexAttributeDescriptions);
	auto inputAssembly = InputAssemblyState::TriangleList();
	auto viewportState = ViewportState::Default();
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
		vk::DynamicState::eScissor
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
std::unordered_map<std::string, Shader> PipelineStateManager::CreateShaders(vk::Device device)
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
