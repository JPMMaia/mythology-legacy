#pragma once

#include "targetver.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <wrl.h>
#include <wrl/client.h>
#include <concrt.h>

#include <d3d12.h>
#include <dxgi1_5.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <pix3.h>
#include <DDSTextureLoader.h>
#include <d3dx12.h>

#if defined(_DEBUG)
#include <dxgidebug.h>
#endif

#include <array>
#include <cstdint>
#include <cstddef>
#include <deque>
#include <fstream>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <Eigen/Eigen>

#include "d3dx12.h"
#include "Common/Helpers.h"
#include "DirectX12Engine/Utilities/DirectXHelper.h"

#include "RenderEngine/Geometry/MeshData.h"
#include "RenderEngine/Geometry/SkinnedMeshData.h"