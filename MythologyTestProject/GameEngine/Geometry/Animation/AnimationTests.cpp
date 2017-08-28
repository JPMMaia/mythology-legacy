#include "stdafx.h"
#include "CppUnitTest.h"
#include "GameEngine/Component/Meshes/SkinnedModelInstance.h"

using namespace Eigen;
using namespace GameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MythologyTestProject
{
	TEST_CLASS(AnimationTests)
	{
	public:
		TEST_METHOD(AnimationTest1)
		{
			std::vector<Keyframe<Vector3f>> positionKeyframes(2);
			positionKeyframes[0] = { 0.0f, { 1.0f, 2.0f, 3.0f } };
			positionKeyframes[1] = { 1000.0f,{ 0.0f, -2.0f, 5.0f } };

			std::vector<Keyframe<Quaternionf>> rotationKeyframes(1);
			rotationKeyframes[0] = { 0.0f, Quaternionf(1.0f, 0.0f, 0.0f, 0.0f) };

			std::vector<Keyframe<Vector3f>> scaleKeyframes;

			BoneAnimation boneAnimation(positionKeyframes, rotationKeyframes, scaleKeyframes);

			Assert::AreEqual(0.0f, boneAnimation.GetStartTime());
			Assert::AreEqual(1000.0f, boneAnimation.GetEndTime());
			
			Affine3f transform;
			boneAnimation.Interpolate(0.0f, transform);
			{
				Affine3f expectedTransform;
				expectedTransform.fromPositionOrientationScale(positionKeyframes[0].Value, rotationKeyframes[0].Value, Vector3f(1.0f, 1.0f, 1.0f));
				Assert::IsTrue(expectedTransform.isApprox(transform));
			}

			boneAnimation.Interpolate(1000.0f, transform);
			{
				Affine3f expectedTransform;
				expectedTransform.fromPositionOrientationScale(positionKeyframes[1].Value, rotationKeyframes[0].Value, Vector3f(1.0f, 1.0f, 1.0f));
				Assert::IsTrue(expectedTransform.isApprox(transform));
			}

			boneAnimation.Interpolate(500.0f, transform);
			{
				Affine3f expectedTransform;
				expectedTransform.fromPositionOrientationScale(Vector3f(0.5f, 0.0f, 4.0f), rotationKeyframes[0].Value, Vector3f(1.0f, 1.0f, 1.0f));
				Assert::IsTrue(expectedTransform.isApprox(transform));
			}
		}

		TEST_METHOD(AnimationTest2)
		{
			std::vector<BoneAnimation> boneAnimations;
			boneAnimations.reserve(2);

			{
				std::vector<Keyframe<Vector3f>> positionKeyframes(3);
				positionKeyframes[0] = { 0.0f,{ 1.0f, 2.0f, 3.0f } };
				positionKeyframes[1] = { 1000.0f,{ 0.0f, -2.0f, 5.0f } };
				positionKeyframes[2] = { 2000.0f,{ 0.0f, -2.0f, 5.0f } };

				boneAnimations.emplace_back(positionKeyframes, std::vector<Keyframe<Quaternionf>>(), std::vector<Keyframe<Vector3f>>());
			}

			{
				std::vector<Keyframe<Quaternionf>> rotationKeyframes(2);
				rotationKeyframes[0] = { 0.0f, Quaternionf(1.0f, 0.0f, 0.0f, 0.0f) };
				rotationKeyframes[1] = { 2000.0f, Quaternionf(0.0f, 0.0f, 1.0f, 0.0f) };

				boneAnimations.emplace_back(std::vector<Keyframe<Vector3f>>(), rotationKeyframes, std::vector<Keyframe<Vector3f>>());
			}

			AnimationClip animationClip(boneAnimations);
			Assert::AreEqual(0.0f, animationClip.GetClipStartTime());
			Assert::AreEqual(2000.0f, animationClip.GetClipEndTime());

			std::vector<Affine3f> transforms;
			animationClip.Interpolate(1500.0f, transforms);

			{
				Affine3f expectedTransform;
				expectedTransform.fromPositionOrientationScale(Vector3f(0.0f, -2.0f, 5.0f), Quaternionf::Identity(), Vector3f(1.0f, 1.0f, 1.0f));
				Assert::IsTrue(expectedTransform.isApprox(transforms[0]));
			}
			{
				Affine3f expectedTransform;
				expectedTransform.fromPositionOrientationScale(Vector3f(0.0f, 0.0f, 0.0f), Quaternionf(1.0f, 0.0f, 0.0f, 0.0f).slerp(1500.0f / 2000.0f, Quaternionf(0.0f, 0.0f, 1.0f, 0.0f)), Vector3f(1.0f, 1.0f, 1.0f));
				Assert::IsTrue(expectedTransform.isApprox(transforms[1]));
			}
		}

		TEST_METHOD(AnimationTest3)
		{
			// 0 - Armature -> Pelvis
			// 1 - Pelvis -> Spine
			// 2 - Spine -> RightArm, LeftArm, Head
			// 3 - RightArm -> RightHand
			// 4 - RightHand
			// 5 - LeftArm -> LeftHand
			// 6 - LeftHand
			// 7 - Head
			std::vector<std::int8_t> boneHierarchy =
			{
				-1, 0, 1, 2, 3, 2, 3, 2
			};
			std::vector<Affine3f> boneTransforms(8);
			for (std::size_t i = 0; i < boneTransforms.size(); ++i)
			{
				auto& transform = boneTransforms[i];
				transform.fromPositionOrientationScale(Vector3f(i * 1.0f, i * 2.0f, i * 3.0f), Quaternionf::FromTwoVectors(Vector3f(0.0f, 1.0f, 0.0f), Vector3f(1.0f, 0.0f, 0.0f)), Vector3f(1.0f, 1.0f, 1.0f));
			}

			std::unordered_map<std::string, AnimationClip> animations;
			{
				std::vector<Keyframe<Vector3f>> positionKeyframes = { { 0.0f, Vector3f(0.0f, 0.0f, 0.0f) } };
				std::vector<Keyframe<Quaternionf>> rotationKeyframes = { { 0.0f, Quaternionf::Identity() } };
				std::vector<Keyframe<Vector3f>> scaleKeyframes = { { 0.0f, Vector3f(1.0f, 1.0f, 1.0f) } };;
				AnimationClip testClip(std::vector<BoneAnimation>(boneHierarchy.size(), BoneAnimation(positionKeyframes, rotationKeyframes, scaleKeyframes)));
				animations.emplace("test", std::move(testClip));
			}

			Affine3f meshToBoneRootMatrix;
			meshToBoneRootMatrix.fromPositionOrientationScale(Vector3f(-1.0f, 3.0f, 3.0f), Quaternionf::FromTwoVectors(Vector3f(0.0f, 0.0f, 1.0f), Vector3f(1.0f, 0.0f, 0.0f)), Vector3f(1.0f, 1.0f, 1.0f));

			SkinnedModelInstance skinnedModelInstace(Armature(boneHierarchy, boneTransforms, animations), meshToBoneRootMatrix);

			Common::Timer timer(std::chrono::milliseconds(12));
			skinnedModelInstace.FrameUpdate(timer);

			const auto& finalTransforms = skinnedModelInstace.GetFinalTransforms();
			auto identity = Affine3f::Identity();
			for (const auto& transform : finalTransforms)
			{
				Assert::IsTrue(transform.isApprox(identity));
			}
		}

		TEST_METHOD(AnimationTest4)
		{
			// TODO test armature update
		}
	};
}
