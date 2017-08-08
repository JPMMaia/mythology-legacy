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
		~CameraComponent()
		{
			int i = 0;
		}

	public:
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
		static Matrix BuildProjectionMatrix(float aspectRatio, float fovAngleY, float nearZ, float farZ, AlignedMatrixCR orientationMatrix);

	private:
		float m_aspectRatio;
		float m_fovAngleY;
		float m_nearZ, m_farZ;
		Matrix m_viewMatrix;
		Matrix m_projectionMatrix;

	public:
		void* operator new(std::size_t size)
		{
			return s_storage.allocate(size);
		}
		void operator delete(void* pointer, std::size_t size) noexcept
		{
			s_storage.deallocate(reinterpret_cast<CameraComponent*>(pointer), size);
		}
		static StandardAllocator<CameraComponent> s_storage;
	};
}
