#pragma once

#include "d3dx12.h"

#include <array>

namespace DirectX12Engine
{
	namespace Samplers
	{
		std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();
	}
}
