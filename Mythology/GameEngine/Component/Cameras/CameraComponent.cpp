#include "CameraComponent.h"

using namespace GameEngine;

StandardAllocator<CameraComponent> CameraComponent::s_storage;

CameraComponent::CameraComponent() :
	m_aspectRatio(16.0f / 9.0f),
	m_fovAngleY(60.0f),
	m_nearZ(0.0625f),
	m_farZ(50.0f),
	m_viewMatrix(Matrix::Identity()),
	m_projectionMatrix(BuildProjectionMatrix(m_aspectRatio, m_fovAngleY, m_nearZ, m_farZ, Matrix::Identity()))
{
}
CameraComponent::CameraComponent(float aspectRatio, float fovAngleY, float nearZ, float farZ, AlignedMatrixCR orientationMatrix) :
	m_aspectRatio(aspectRatio),
	m_fovAngleY(fovAngleY),
	m_nearZ(nearZ),
	m_farZ(farZ),
	m_viewMatrix(Matrix::Identity()),
	m_projectionMatrix(BuildProjectionMatrix(aspectRatio, fovAngleY, nearZ, farZ, orientationMatrix))
{
}

void CameraComponent::FixedUpdate(const Common::Timer& timer)
{
	BaseComponent::FixedUpdate(timer);

	const auto& transform = GetTransform();
	m_viewMatrix = BuildViewMatrix(transform.GetWorldPosition(), transform.GetWorldRotation());
}

CameraComponent::MatrixCR CameraComponent::GetViewMatrix() const
{
	return m_viewMatrix;
}
CameraComponent::MatrixCR CameraComponent::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}

float CameraComponent::GetAspectRatio() const
{
	return m_aspectRatio;
}
void CameraComponent::SetAspectRatio(float aspectRatio)
{
	m_aspectRatio = aspectRatio;
}

float CameraComponent::GetFovAngleY() const
{
	return m_fovAngleY;
}
void CameraComponent::SetFovAngleY(float fovAngleY)
{
	m_fovAngleY = fovAngleY;
}

float CameraComponent::GetNearZ() const
{
	return m_nearZ;
}
void CameraComponent::SetNearZ(float nearZ)
{
	m_nearZ = nearZ;
}

float CameraComponent::GetFarZ() const
{
	return m_farZ;
}
void CameraComponent::SetFarZ(float farZ)
{
	m_farZ = farZ;
}

void CameraComponent::SetOrientationMatrix(AlignedMatrixCR orientationMatrix)
{
	m_projectionMatrix = BuildProjectionMatrix(m_aspectRatio, m_fovAngleY, m_nearZ, m_farZ, orientationMatrix);
}

CameraComponent::Matrix CameraComponent::BuildViewMatrix(Vector3CR position, QuaternionCR rotation)
{
	auto matrix = (rotation.toRotationMatrix() * Eigen::Translation3f(position)).inverse();

	return matrix;
}
CameraComponent::Matrix CameraComponent::BuildProjectionMatrix(float aspectRatio, float fovAngleY, float nearZ, float farZ, AlignedMatrixCR orientationMatrix)
{
	auto yScale = 1.0f / std::tan(fovAngleY / 2.0f);
	auto xScale = yScale / aspectRatio;

#if defined(USING_DIRECTX)
	auto depthNear = 0.0f;
#else
	auto depthNear = -1.0f;
#endif
	auto depthFar = 1.0f;

	auto perspectiveMatrix(AlignedMatrix::Identity());
	perspectiveMatrix(0, 0) = xScale;
	perspectiveMatrix(1, 1) = yScale;
	perspectiveMatrix(2, 2) = farZ / (nearZ - farZ);
	perspectiveMatrix(3, 2) = -1.0f;
	perspectiveMatrix(2, 3) = (nearZ * farZ) / (nearZ - farZ);
	perspectiveMatrix(3, 3) = 0.0f;
	//perspectiveMatrix(2, 3) = (depthFar - depthNear) * (farZ * nearZ) / (farZ - nearZ);

	return orientationMatrix * perspectiveMatrix;
}
