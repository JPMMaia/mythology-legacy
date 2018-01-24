#version 450

// Input:
layout(location = 0) in vec2 in_positionW;
layout(location = 1) in vec3 in_color;

// Output:
layout(location = 0) out vec3 out_color;

void main() 
{
	// Output values:
	gl_Position = vec4(in_positionW, 0.0f, 1.0f);
	out_color = in_color;
}
