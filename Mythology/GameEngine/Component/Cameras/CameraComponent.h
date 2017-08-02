#pragma once

#include "GameEngine/Component/Base/BaseComponent.h"

namespace GameEngine
{
	class CameraComponent : public BaseComponent
	{
	public:
		using Vector3 = Eigen::Vector3f;
		using Vector3CR = const Vector3&;
		using Quaternion = Eigen::Quaternionf;
		using QuaternionCR = const Quaternion&;
		using Matrix = Eigen::Affine3f;
		using MatrixCR = const Matrix&;

	public:
		CameraComponent();
		CameraComponent(float aspectRatio, float fovAngleY, float nearZ, float farZ, MatrixCR orientationMatrix);

		void Update();

		MatrixCR GetViewMatrix() const;
		MatrixCR GetProjectionMatrix() const;

		float GetNearZ() const;
		void SetNearZ(float value);

		float GetFarZ() const;
		void SetFarZ(float value);

		void SetAspectRatio(float aspectRatio);

	private:
		void InitializeProjectionMatrix(MatrixCR orientationMatrix);
	};
}
