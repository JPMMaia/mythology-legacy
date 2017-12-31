#include "pch.h"
#include "CppUnitTest.h"
#include "GameEngine/Geometry/Importer/SceneImporter.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace Assimp;
using namespace Eigen;
using namespace GameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MythologyTestProject
{
	TEST_CLASS(SceneImporterTests)
	{
	private:
		Affine3f AssimpToEigen(const aiMatrix4x4& assimpMatrix)
		{
			const auto& m = assimpMatrix;

			Matrix4f eigenMatrix;
			eigenMatrix <<
				m.a1, m.a2, m.a3, m.a4,
				m.b1, m.b2, m.b3, m.b4,
				m.c1, m.c2, m.c3, m.c4,
				m.d1, m.d2, m.d3, m.d4;

			return Affine3f(eigenMatrix);
		}

	public:
		TEST_METHOD(SceneImporterTest1)
		{
			SceneImporter::ImportedScene scene;
			SceneImporter::Import(L"test.fbx", scene);

			Assert::AreEqual(std::size_t(1), scene.Objects.size());
			Assert::AreEqual(std::size_t(2), scene.Materials.size());
			Assert::AreEqual(std::size_t(1), scene.Armatures.size());
			
			const auto& object = scene.Objects.front();
			Assert::AreEqual(std::size_t(2), object.Geometries.size());
			Assert::AreEqual(std::size_t(0), object.ArmatureIndex);
			Assert::IsTrue(object.IsAnimated);
			
			
			// Test mesh to bone root matrix:
			{
				Importer importer;
				auto flags =
					aiProcess_CalcTangentSpace |
					aiProcess_Triangulate |
					aiProcess_GenNormals |
					aiProcess_GenUVCoords |
					aiProcess_FindInvalidData |
					aiProcess_JoinIdenticalVertices |
					aiProcess_LimitBoneWeights |
					aiProcess_ImproveCacheLocality |
					aiProcess_ValidateDataStructure |
					aiProcess_FlipUVs |
					aiProcess_SortByPType;
				auto scene2 = importer.ReadFile("test.fbx", flags);
				Assert::IsTrue(scene2 != nullptr);

				auto armatureNode = scene2->mRootNode->FindNode("Armature");
				auto meshNode = scene2->mRootNode->FindNode("PersonObject");

				// Armature^(-1) * Mesh
				auto armatureTransform = AssimpToEigen(armatureNode->mTransformation);
				auto meshTransform = AssimpToEigen(meshNode->mTransformation);
				auto meshToParentOfBoneRoot = meshTransform;
				Assert::IsTrue(meshToParentOfBoneRoot.isApprox(object.MeshToParentOfBoneRoot));

				// Find the RightHand bone:
				const aiBone* rightHandBone = nullptr;
				for (std::size_t meshIndex = 0; meshIndex < scene2->mNumMeshes && rightHandBone == nullptr; ++meshIndex)
				{
					const auto& mesh = *scene2->mMeshes[meshIndex];

					for (std::size_t boneIndex = 0; boneIndex < mesh.mNumBones; ++boneIndex)
					{
						const auto& bone = *mesh.mBones[boneIndex];
						if ("RightHand" == std::string(bone.mName.C_Str()))
						{
							rightHandBone = &bone;
							break;
						}
					}
				}

				// RightHand^(-1) * RightArm^(-1) * Spine^(-1) * Pelvis^(-1) * Armature^(-1) * Mesh
				auto pelvisTransform = AssimpToEigen(armatureNode->FindNode("Pelvis")->mTransformation);
				auto spineTransform = AssimpToEigen(armatureNode->FindNode("Spine")->mTransformation);
				auto rightArmTransform = AssimpToEigen(armatureNode->FindNode("RightArm")->mTransformation);
				auto rightHandTransform = AssimpToEigen(armatureNode->FindNode("RightHand")->mTransformation);
				auto rightHandOffsetMatrix = rightHandTransform.inverse() * rightArmTransform.inverse() * spineTransform.inverse() * pelvisTransform.inverse() * armatureTransform.inverse() * meshToParentOfBoneRoot;
				Assert::IsTrue(rightHandOffsetMatrix.isApprox(AssimpToEigen(rightHandBone->mOffsetMatrix), 0.1f));
			}
		}
	};
}
