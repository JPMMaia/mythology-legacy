#include "pch.h"
#include "CppUnitTest.h"
#include "GameEngine/Component/Animations/SkinnedAnimationComponent.h"

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
				std::vector<Keyframe<Vector3f>> positionKeyframes(2);
				positionKeyframes[0] = { 0.0f,{ 1.0f, 2.0f, 3.0f } };
				positionKeyframes[1] = { 1000.0f,{ 0.0f, -2.0f, 5.0f } };

				boneAnimations.emplace_back(positionKeyframes, std::vector<Keyframe<Quaternionf>>(), std::vector<Keyframe<Vector3f>>());
			}

			{
				std::vector<Keyframe<Quaternionf>> rotationKeyframes(2);
				rotationKeyframes[0] = { 0.0f, Quaternionf(1.0f, 0.0f, 0.0f, 0.0f) };
				rotationKeyframes[1] = { 3000.0f, Quaternionf(0.0f, 0.0f, 1.0f, 0.0f) };

				boneAnimations.emplace_back(std::vector<Keyframe<Vector3f>>(), rotationKeyframes, std::vector<Keyframe<Vector3f>>());
			}

			AnimationClip animationClip(boneAnimations);
			Assert::AreEqual(0.0f, animationClip.GetClipStartTime());
			Assert::AreEqual(3000.0f, animationClip.GetClipEndTime());

			std::vector<Affine3f> transforms;
			animationClip.Interpolate(1500.0f, transforms);

			{
				Affine3f expectedTransform;
				expectedTransform.fromPositionOrientationScale(Vector3f(0.0f, -2.0f, 5.0f), Quaternionf::Identity(), Vector3f(1.0f, 1.0f, 1.0f));
				Assert::IsTrue(expectedTransform.isApprox(transforms[0]));
			}
			{
				Affine3f expectedTransform;
				expectedTransform.fromPositionOrientationScale(Vector3f(0.0f, 0.0f, 0.0f), Quaternionf(1.0f, 0.0f, 0.0f, 0.0f).slerp(1500.0f / 3000.0f, Quaternionf(0.0f, 0.0f, 1.0f, 0.0f)), Vector3f(1.0f, 1.0f, 1.0f));
				Assert::IsTrue(expectedTransform.isApprox(transforms[1]));
			}
		}

		TEST_METHOD(AnimationTest3)
		{
			std::vector<std::int8_t> boneHierarchy =
			{
				-1, 0
			};
			std::vector<Affine3f> boneTransforms(boneHierarchy.size());
			boneTransforms[0].fromPositionOrientationScale(Vector3f(0.0f, 1.0f, 0.0f), Quaternionf::Identity(), Vector3f(1.0f, 1.0f, 1.0f));
			boneTransforms[1].fromPositionOrientationScale(Vector3f(0.0f, 2.0f, 0.0f), Quaternionf::Identity(), Vector3f(1.0f, 1.0f, 1.0f));

			std::unordered_map<std::string, AnimationClip> animations;
			{
				std::vector<BoneAnimation> boneAnimations(boneHierarchy.size());
				std::vector<Keyframe<Vector3f>> positionKeyframes(2);
				positionKeyframes[0] = { 0.0f, Vector3f(0.0f, 1.0f, 0.0f) };
				positionKeyframes[1] = { 1000.0f, Vector3f(0.0f, 3.0f, 0.0f) };
				std::vector<Keyframe<Quaternionf>> rotationKeyframes;
				std::vector<Keyframe<Vector3f>> scaleKeyframes;
				boneAnimations[0] = BoneAnimation(positionKeyframes, rotationKeyframes, scaleKeyframes);

				positionKeyframes[0] = {0.0f, Vector3f(0.0f, 2.0f, 0.0f) };
				positionKeyframes[1] = { 1000.0f, Vector3f(0.0f, 3.0f, 0.0f) };
				boneAnimations[1] = BoneAnimation(positionKeyframes, rotationKeyframes, scaleKeyframes);

				AnimationClip testClip(boneAnimations);
				animations.emplace("test", std::move(testClip));
			}

			Affine3f meshToParentOfBoneRootMatrix;
			meshToParentOfBoneRootMatrix.fromPositionOrientationScale(Vector3f(-1.0f, 3.0f, 3.0f), Quaternionf::FromTwoVectors(Vector3f(0.0f, 0.0f, 1.0f), Vector3f(1.0f, 0.0f, 0.0f)), Vector3f(1.0f, 1.0f, 1.0f));

			SkinnedAnimationComponent skinnedModelInstace(std::make_shared<Armature>(boneHierarchy, boneTransforms, animations), meshToParentOfBoneRootMatrix);

			Common::Timer timer(std::chrono::milliseconds(12));
			timer.SetDeltaTime(std::chrono::milliseconds(0));
			skinnedModelInstace.FrameUpdate(timer);

			const auto& finalTransforms = skinnedModelInstace.GetFinalTransforms();
			Assert::AreEqual(boneHierarchy.size(), finalTransforms.size());
			
			{
				Affine3f expectedTransform;
				expectedTransform.fromPositionOrientationScale(Vector3f(0.0f, 1.0f, 0.0f), Quaternionf::Identity(), Vector3f(1.0f, 1.0f, 1.0f));
				expectedTransform = meshToParentOfBoneRootMatrix.inverse() * expectedTransform * boneTransforms[0].inverse() * meshToParentOfBoneRootMatrix;
				Assert::IsTrue(expectedTransform.isApprox(finalTransforms[0]));
			}

			{
				Affine3f expectedTransform;
				expectedTransform.fromPositionOrientationScale(Vector3f(0.0f, 3.0f, 0.0f), Quaternionf::Identity(), Vector3f(1.0f, 1.0f, 1.0f));
				expectedTransform = meshToParentOfBoneRootMatrix.inverse() * expectedTransform * boneTransforms[1].inverse() * boneTransforms[0].inverse() * meshToParentOfBoneRootMatrix;
				Assert::IsTrue(expectedTransform.isApprox(finalTransforms[1]));
			}

			timer.SetDeltaTime(std::chrono::milliseconds(1000));
			skinnedModelInstace.FrameUpdate(timer);

			{
				Affine3f expectedTransform;
				expectedTransform.fromPositionOrientationScale(Vector3f(0.0f, 3.0f, 0.0f), Quaternionf::Identity(), Vector3f(1.0f, 1.0f, 1.0f));
				expectedTransform = meshToParentOfBoneRootMatrix.inverse() * expectedTransform * boneTransforms[0].inverse() * meshToParentOfBoneRootMatrix;
				Assert::IsTrue(expectedTransform.isApprox(finalTransforms[0]));
			}

			{
				Affine3f expectedTransform;
				expectedTransform.fromPositionOrientationScale(Vector3f(0.0f, 6.0f, 0.0f), Quaternionf::Identity(), Vector3f(1.0f, 1.0f, 1.0f));
				expectedTransform = meshToParentOfBoneRootMatrix.inverse() * expectedTransform * boneTransforms[1].inverse() * boneTransforms[0].inverse() * meshToParentOfBoneRootMatrix;
				Assert::IsTrue(expectedTransform.isApprox(finalTransforms[1]));
			}
		}
	};
}
