#include "CameraComponent.h"

using namespace GameEngine;

CameraComponent::CameraComponent() :
	m_aspectRatio(16.0f / 9.0f),
	m_fovAngleY(60.0f),
	m_nearZ(0.0625f),
	m_farZ(50.0f),
	m_viewMatrix(Matrix::Identity()),
	m_projectionMatrix(BuildProjectionMatrix(m_aspectRatio, m_fovAngleY, m_nearZ, m_farZ, Matrix::Identity()))
{
}
CameraComponent::CameraComponent(float aspectRatio, float fovAngleY, float nearZ, float farZ, MatrixCR orientationMatrix) :
	m_aspectRatio(aspectRatio),
	m_fovAngleY(fovAngleY),
	m_nearZ(nearZ),
	m_farZ(farZ),
	m_viewMatrix(Matrix::Identity()),
	m_projectionMatrix(BuildProjectionMatrix(aspectRatio, fovAngleY, nearZ, farZ, orientationMatrix))
{
}

void CameraComponent::Update()
{
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

CameraComponent::Matrix CameraComponent::BuildViewMatrix(Vector3CR position, QuaternionCR rotation)
{
	return rotation.toRotationMatrix() * Eigen::Translation3f(-position);
}
CameraComponent::Matrix CameraComponent::BuildProjectionMatrix(float aspectRatio, float fovAngleY, float nearZ, float farZ, MatrixCR orientationMatrix)
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

	return orientationMatrix * perspectiveMatrix;
}
