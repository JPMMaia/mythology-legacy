#include "stdafx.h"
#include "CppUnitTest.h"
#include "GameEngine/Geometry/MeshGenerator.h"
#include "GameEngine/Geometry/MeshData.h"
#include "Libraries/tue/vec.hpp"

#include <DirectXMath.h>

using namespace GameEngine;
using namespace DirectX;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using XMMeshData = MeshData<XMFLOAT2, XMFLOAT3, uint32_t>;
using TueMeshData = MeshData<tue::fvec2, tue::fvec3, uint32_t>;

using XMMeshGenerator = MeshGenerator<XMMeshData>;
using TueMeshGenerator = MeshGenerator<TueMeshData>;

namespace MythologyTestProject
{
	TEST_CLASS(MeshDataTests)
	{
	public:

		TEST_METHOD(MeshDataTestXM)
		{
			XMMeshGenerator::CreateRectangle(0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
			XMMeshGenerator::CreateBox(1.0f, 1.0f, 1.0f, 1);
		}

		TEST_METHOD(MeshDataTestTue)
		{
			TueMeshGenerator::CreateRectangle(0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
			TueMeshGenerator::CreateBox(1.0f, 1.0f, 1.0f, 1);
		}
	};
}