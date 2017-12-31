#include "pch.h"
#include "CppUnitTest.h"
#include "GameEngine/Component/Cameras/CameraComponent.h"
#include "Common/Timers/Timer.h"

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
			auto yScale = 1.0f / std::tan(fovAngleY / 2.0f);
			auto xScale = yScale / aspectRatio;
			auto nearDepth = -1.0f;
			auto farDepth = 1.0f;

			auto perspectiveMatrix(Matrix::Identity());
			perspectiveMatrix(0, 0) = xScale;
			perspectiveMatrix(1, 1) = yScale;
			perspectiveMatrix(2, 2) = (farDepth * farZ - nearDepth * nearZ) / (nearZ - farZ);
			perspectiveMatrix(3, 2) = -1.0f;
			perspectiveMatrix(2, 3) = (farDepth - nearDepth) * (nearZ * farZ) / (nearZ - farZ);
			perspectiveMatrix(3, 3) = 0.0f;

			return perspectiveMatrix;
		}

		static Matrix CalculateViewMatrix(const Vector3f& position, const Quaternionf& rotation)
		{
			return (Translation3f(position) * rotation.toRotationMatrix()).inverse();
		}

		TEST_METHOD(DefaultConstructor)
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

		TEST_METHOD(CustomConstructor)
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

		TEST_METHOD(ViewMatrix)
		{
			const auto pi = std::acos(-1.0f);

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
	};

	const CameraComponent::Matrix CameraComponentTests::s_rotation90(AngleAxisf(acos(-1.0f) / 2.0f, Vector3f::UnitZ()).matrix());
}
