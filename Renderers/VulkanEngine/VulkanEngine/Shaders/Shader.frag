#version 450
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_separate_shader_objects : enable

// Input:
layout(location = 0) in vec3 in_color;

// Output:
layout(location = 0) out vec4 out_color;

void main() 
{
	out_color = vec4(1.0f, 1.0f, 1.0f, 1.0);
}
