#include "pch.h"
#include "PipelineStateManager.h"

using namespace std;
using namespace VulkanEngine;

PipelineStateManager::PipelineStateManager(VkDevice device) :
	m_device(device)
{
	CreateShaders();
}

void PipelineStateManager::CreateShaders()
{
	vector<pair<string, wstring>> shaders = 
	{
		{ "StandardVertexShader", L"Shaders/vert.spv" },
		{ "FragmentVertexShader", L"Shaders/frag.spv" }
	};

	for (const auto& shader : shaders)
		CreateShader(shader.first, shader.second);
}
void PipelineStateManager::CreateShader(const std::string& name, const std::wstring& filename)
{
	m_shaders.emplace(
		std::piecewise_construct,
		std::forward_as_tuple(name),
		std::forward_as_tuple(m_device, filename)
	);
}
