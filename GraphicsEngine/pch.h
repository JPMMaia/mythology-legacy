#pragma once

#include "targetver.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <wrl.h>
#include <wrl/client.h>
#include <dxgi1_5.h>
#include <d3d12.h>
#include <pix.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <memory>
#include <vector>
#include <concrt.h>
#include <cstdint>
#include <string>

#if defined(_DEBUG)
#include <dxgidebug.h>
#endif
