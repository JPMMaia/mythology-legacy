#include "pch.h"
#include "CameraComponent.h"

using namespace Eigen;
using namespace GameEngine;

CameraComponent::CameraComponent() :
	m_aspectRatio(16.0f / 9.0f),
	m_fovAngleY(60.0f),
	m_nearZ(0.0625f),
	m_farZ(50.0f),
	m_projectionMatrix(BuildProjectionMatrix(m_aspectRatio, m_fovAngleY, m_nearZ, m_farZ, Matrix::Identity()))
{
}
CameraComponent::CameraComponent(float aspectRatio, float fovAngleY, float nearZ, float farZ, AlignedMatrixCR orientationMatrix) :
	m_aspectRatio(aspectRatio),
	m_fovAngleY(fovAngleY),
	m_nearZ(nearZ),
	m_farZ(farZ),
	m_projectionMatrix(BuildProjectionMatrix(aspectRatio, fovAngleY, nearZ, farZ, orientationMatrix))
{
}

CameraComponent::Matrix CameraComponent::GetViewMatrix() const
{
	const auto& transform = GetTransform();
	return BuildViewMatrix(transform);
}
CameraComponent::Matrix CameraComponent::GetProjectionMatrix() const
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

CameraComponent::Matrix CameraComponent::BuildViewMatrix(const TransformComponent& transform)
{
	auto eyePosition = transform.GetWorldPosition();
	auto eyeDirection = transform.GetWorldZ();
	auto upDirection = transform.GetWorldY();

#if defined(USING_DIRECTX)
	eyeDirection = -eyeDirection;
#endif

	auto zAxis = eyeDirection.normalized();
	auto xAxis = upDirection.cross(zAxis).normalized();
	auto yAxis = zAxis.cross(xAxis);

	Matrix4f matrix;
	matrix <<
		xAxis.x(), xAxis.y(), xAxis.z(), -xAxis.dot(eyePosition),
		yAxis.x(), yAxis.y(), yAxis.z(), -yAxis.dot(eyePosition),
		zAxis.x(), zAxis.y(), zAxis.z(), -zAxis.dot(eyePosition),
		0.0f, 0.0f, 0.0f, 1.0f;

	return Affine3f(matrix);

}
CameraComponent::Matrix CameraComponent::BuildProjectionMatrix(float aspectRatio, float fovAngleY, float nearZ, float farZ, AlignedMatrixCR orientationMatrix)
{
	auto yScale = 1.0f / std::tan(fovAngleY / 2.0f);
	auto xScale = yScale / aspectRatio;

#if defined(USING_DIRECTX)
	auto nearDepth = 0.0f;
#else
	auto nearDepth = -1.0f;
#endif
	auto farDepth = 1.0f;

	auto perspectiveMatrix(AlignedMatrix::Identity());
	perspectiveMatrix(0, 0) = xScale;
	perspectiveMatrix(1, 1) = yScale;
	perspectiveMatrix(2, 2) = (farDepth * farZ - nearDepth * nearZ) / (nearZ - farZ);
	perspectiveMatrix(3, 2) = -1.0f;
	perspectiveMatrix(2, 3) = (farDepth - nearDepth) * (nearZ * farZ) / (nearZ - farZ);
	perspectiveMatrix(3, 3) = 0.0f;

	return orientationMatrix * perspectiveMatrix;
}
