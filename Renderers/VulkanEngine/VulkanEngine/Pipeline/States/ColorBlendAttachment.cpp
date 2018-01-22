#include "pch.h"
#include "ColorBlendAttachment.h"

using namespace VulkanEngine;

vk::PipelineColorBlendAttachmentState ColorBlendAttachment::Default()
{
	return vk::PipelineColorBlendAttachmentState();
}
