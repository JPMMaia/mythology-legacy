#include "stdafx.h"
#include "CppUnitTest.h"
#include "GameEngine/Component/Meshes/InstancedMeshComponent.h"
#include "GameEngine/Geometry/Primitives/BoxGeometry.h"
#include "GameEngine/Component/Meshes/MeshComponent.h"

using namespace GameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MythologyTestProject
{
	TEST_CLASS(MeshComponentTests)
	{
	public:
		TEST_METHOD(MeshComponentTest1)
		{
			/*auto meshPointer = new MeshComponent<BoxGeometry>(BoxGeometry(1.0f, 1.0f, 1.0f, 0));
			std::shared_ptr<MeshComponent<BoxGeometry>> mesh(meshPointer);

			Assert::AreEqual(std::size_t(0), mesh->GetInstanceCount());

			auto instance1 = std::make_shared<InstancedMeshComponent<BoxGeometry>>(mesh);
			Assert::AreEqual(std::size_t(1), mesh->GetInstanceCount());

			{
				InstanceData data;
				data.ModelMatrix = Eigen::Affine3f::Identity();
				data.MaterialIndex = 1;
				instance1->SetInstanceData(data);

				const auto& dataInsideMesh = mesh->GetInstanceData(0);
				Assert::IsTrue(dataInsideMesh.ModelMatrix.isApprox(Eigen::Affine3f::Identity()));
				Assert::AreEqual(std::uint32_t(1), dataInsideMesh.MaterialIndex);
			}

			{
				auto instance2 = std::make_shared<InstancedMeshComponent<BoxGeometry>>(mesh);
				auto instance3 = std::make_shared<InstancedMeshComponent<BoxGeometry>>(mesh);
				Assert::AreEqual(std::size_t(3), mesh->GetInstanceCount());
			}
			Assert::AreEqual(std::size_t(1), mesh->GetInstanceCount());

			{
				auto instance4 = std::make_shared<InstancedMeshComponent<BoxGeometry>>(mesh);
				auto instance5 = std::make_shared<InstancedMeshComponent<BoxGeometry>>(mesh);

				InstanceData data;
				data.MaterialIndex = 5;
				instance5->SetInstanceData(data);

				// Destroy instance and check if instance5 is still valid:
				instance4.reset();
				Assert::AreEqual(std::uint32_t(5), instance5->GetInstanceData().MaterialIndex);
			}*/
		}
	};
}
