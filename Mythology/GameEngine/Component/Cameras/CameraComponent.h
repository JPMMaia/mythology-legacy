#pragma once

#include "GameEngine/Memory/StandardAllocator.h"
#include "Common/MemoryPool/MemoryPool.h"
#include "GameEngine/Component/Base/BaseComponent.h"

namespace GameEngine
{
	class CameraComponent : public BaseComponent
	{
		friend StandardAllocator<CameraComponent>;

	public:
		using Vector3 = Eigen::Vector3f;
		using Vector3CR = const Vector3&;
		using Quaternion = Eigen::Quaternionf;
		using QuaternionCR = const Quaternion&;
		using Matrix = Eigen::Transform<float, 3, Eigen::Projective>;
		using MatrixCR = const Matrix&;

	public:
		CameraComponent();
		CameraComponent(float aspectRatio, float fovAngleY, float nearZ, float farZ, MatrixCR orientationMatrix);

		void Update();

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

	private:
		static Matrix BuildViewMatrix(Vector3CR position, QuaternionCR rotation);
		static Matrix BuildProjectionMatrix(float aspectRatio, float fovAngleY, float nearZ, float farZ, MatrixCR orientationMatrix);

	private:
		float m_aspectRatio;
		float m_fovAngleY;
		float m_nearZ, m_farZ;
		Matrix m_viewMatrix;
		Matrix m_projectionMatrix;

	public:
		static Common::MemoryPool<CameraComponent, 2>& GetStorage()
		{
			return s_storage;
		}

	private:
		static Common::MemoryPool<CameraComponent, 2> s_storage;
	};
}
