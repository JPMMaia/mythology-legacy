#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"
#include "GameEngine/Memory/StandardAllocator.h"

namespace GameEngine
{
	class CameraComponent : public BaseComponent
	{
	public:
		using Vector3 = Eigen::Vector3f;
		using Vector3CR = const Vector3&;
		using Quaternion = Eigen::Quaternionf;
		using QuaternionCR = const Quaternion&;
		using Matrix = Eigen::Transform<float, 3, Eigen::Projective, Eigen::DontAlign>;
		using MatrixCR = const Matrix&;
		using AlignedMatrix = Eigen::Transform<float, 3, Eigen::Projective>;
		using AlignedMatrixCR = const AlignedMatrix&;

	public:
		CameraComponent();
		CameraComponent(float aspectRatio, float fovAngleY, float nearZ, float farZ, AlignedMatrixCR orientationMatrix);

	public:
		void FixedUpdate(const Common::Timer& timer) override;

		MatrixCR GetViewMatrix() const;
		MatrixCR GetProjectionMatrix() const;

		float GetAspectRatio() const;
		void SetAspectRatio(float aspectRatio);

		float GetFovAngleY() const;
		void SetFovAngleY(float fovAngleY);

		float GetNearZ() const;
		void SetNearZ(float nearZ);

		float GetFarZ() const;
		void SetFarZ(float farZ);

		void SetOrientationMatrix(AlignedMatrixCR orientationMatrix);

	private:
		static Matrix BuildViewMatrix(const TransformComponent& transform);
		static Matrix BuildProjectionMatrix(float aspectRatio, float fovAngleY, float nearZ, float farZ, AlignedMatrixCR orientationMatrix);

	private:
		float m_aspectRatio;
		float m_fovAngleY;
		float m_nearZ, m_farZ;
		Matrix m_viewMatrix;
		Matrix m_projectionMatrix;

		DEFINE_ALLOCATOR(CameraComponent);
	};
}
