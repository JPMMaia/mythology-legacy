#version 450

layout(set = 0, binding = 0) uniform InstanceData
{
	mat4 ModelMatrix;
} in_instanceData;

layout(set = 1, binding = 0) uniform PassData
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
} in_passData;

// Input:
layout(location = 0) in vec2 in_positionL;
layout(location = 1) in vec3 in_color;

// Output:
layout(location = 0) out vec3 out_color;

void main() 
{
	// Calculate position:
	vec4 positionW = in_instanceData.ModelMatrix * vec4(in_positionL, 0.0f, 1.0f);
	vec4 positionV = in_passData.ViewMatrix * positionW;
	gl_Position = in_passData.ProjectionMatrix * positionV;

	// Output color:
	out_color = in_color;
}
