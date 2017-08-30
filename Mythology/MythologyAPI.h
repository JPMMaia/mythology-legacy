#pragma once

#include <array>
#include <deque>
#include <cstdint>
#include <cstddef>
#include <cmath>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

#include "Common/Timer.h"

#include "Eigen/Core"
#include "Eigen/Geometry"

#include "PxPhysicsAPI.h"
#if defined(_DEBUG)
#pragma comment(lib, "PhysX3DEBUG_x64.lib")
#pragma comment(lib, "PhysX3CommonDEBUG_x64.lib")
#pragma comment(lib, "PxFoundationDEBUG_x64.lib")
#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib")
#pragma comment(lib, "PxPvdSDKDEBUG_x64.lib")
#endif
