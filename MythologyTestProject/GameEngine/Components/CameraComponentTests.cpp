#include "stdafx.h"
#include "CppUnitTest.h"
#include "GameEngine/Component/Cameras/CameraComponent.h"
#include "Common/Timer.h"

using namespace Eigen;
using namespace GameEngine;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MythologyTestProject
{
	TEST_CLASS(CameraComponentTests)
	{
	public:
		using Vector3 = CameraComponent::Vector3;
		using Matrix = CameraComponent::Matrix;

	private:
		static const Matrix s_rotation90;

	public:

		static Matrix CalculatePerspectiveMatrix(float aspectRatio, float fovAngleY, float nearZ, float farZ)
		{
			auto top = std::tan(fovAngleY) * nearZ;
			auto bottom = -top;
			auto right = top * aspectRatio;
			auto left = -aspectRatio;

			auto perspectiveMatrix(Matrix::Identity());
			perspectiveMatrix(0, 0) = 2.0f * nearZ / (right - left);
			perspectiveMatrix(1, 1) = 2.0f * nearZ / (top - bottom);
			perspectiveMatrix(0, 2) = (right + left) / (right - left);
			perspectiveMatrix(1, 2) = (top + bottom) / (top - bottom);
			perspectiveMatrix(2, 2) = -(farZ + nearZ) / (farZ - nearZ);
			perspectiveMatrix(3, 2) = -1.0f;
			perspectiveMatrix(2, 3) = -(2.0f * farZ * nearZ) / (farZ - nearZ);
			perspectiveMatrix(3, 3) = 0.0f;

			return perspectiveMatrix;
		}

		static Matrix CalculateViewMatrix(const Vector3f& position, const Quaternionf& rotation)
		{
			return rotation.toRotationMatrix() * Translation3f(-position);
		}

		TEST_METHOD(CameraComponentTest1)
		{
			const auto pi = std::acos(-1.0f);

			// Test default construction:
			{
				auto aspectRatio = 16.0f / 9.0f;
				auto nearZ = 0.0625f;
				auto farZ = 50.0f;
				auto fovAngleY = 60.0f;

				CameraComponent camera;
				Assert::AreEqual(aspectRatio, camera.GetAspectRatio());
				Assert::AreEqual(fovAngleY, camera.GetFovAngleY());
				Assert::AreEqual(nearZ, camera.GetNearZ());
				Assert::AreEqual(farZ, camera.GetFarZ());
				Assert::IsTrue(camera.GetViewMatrix().isApprox(Matrix::Identity()));

				auto perspectiveMatrix = CalculatePerspectiveMatrix(aspectRatio, fovAngleY, nearZ, farZ);
				Assert::IsTrue(camera.GetProjectionMatrix().isApprox(perspectiveMatrix));
			}

			// Test custom constructor and accessors:
			{
				auto aspectRatio = 4.0f / 3.0f;
				auto fovAngleY = 90.0f;
				auto nearZ = 0.125f;
				auto farZ = 100.0f;

				CameraComponent camera(aspectRatio, fovAngleY, nearZ, farZ, s_rotation90);
				Assert::AreEqual(aspectRatio, camera.GetAspectRatio());
				Assert::AreEqual(nearZ, camera.GetNearZ());
				Assert::AreEqual(farZ, camera.GetFarZ());
				Assert::IsTrue(camera.GetViewMatrix().isApprox(Matrix::Identity()));

				auto perspectiveMatrix = CalculatePerspectiveMatrix(aspectRatio, fovAngleY, nearZ, farZ);
				Assert::IsTrue(camera.GetProjectionMatrix().isApprox(s_rotation90 * perspectiveMatrix));
			}

			// Test view matrix:
			{
				Common::Timer timer(std::chrono::milliseconds(12));

				auto aspectRatio = 16.0f / 9.0f;
				auto nearZ = 0.0625f;
				auto farZ = 50.0f;
				auto fovAngleY = 60.0f;

				CameraComponent camera(aspectRatio, fovAngleY, nearZ, farZ, s_rotation90);

				Vector3 position = { 1.0f, -2.0f, 3.0f };
				Quaternionf rotation(AngleAxisf(pi / 2.0f, Vector3::UnitY()));
				camera.GetTransform().SetLocalPosition(position);
				camera.GetTransform().SetLocalRotation(rotation);
				camera.FixedUpdate(timer);

				auto viewMatrix = CalculateViewMatrix(position, rotation);
				Assert::IsTrue(camera.GetViewMatrix().isApprox(viewMatrix));

				Vector3 parentPosition = { 3.0f, 4.0f, 5.0f };
				Quaternionf parentRotation(AngleAxisf(pi / 2.0f, Vector3::UnitZ()));
				auto parent = std::make_shared<TransformComponent>();
				parent->SetLocalPosition(parentPosition);
				parent->SetLocalRotation(parentRotation);

				camera.SetParent(parent, false);
				camera.FixedUpdate(timer);

				auto worldViewMatrix = CalculateViewMatrix({ 5.0f, 5.0f, 8.0f }, parentRotation * rotation);
				Assert::IsTrue(camera.GetViewMatrix().isApprox(worldViewMatrix));
			}
		}
	};

	const CameraComponent::Matrix CameraComponentTests::s_rotation90(AngleAxisf(acos(-1.0f) / 2.0f, Vector3f::UnitZ()).matrix());
}
