#include "pch.h"
#include "CubeMappingCamera.h"

using namespace Eigen;
using namespace GameEngine;

CubeMappingCamera::CubeMappingCamera(const Eigen::Vector3f& eyePosition)
{
	BuildMatrices(eyePosition);
}

const Eigen::Affine3f& CubeMappingCamera::GetViewMatrix(size_t index) const
{
	return m_viewMatrices[index];
}
const Eigen::Affine3f& CubeMappingCamera::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}

void CubeMappingCamera::BuildMatrices(const Eigen::Vector3f& eyePosition)
{
	std::array<Vector3f, 6> eyesDirections =
	{
		Vector3f(1.0f, 0.0f, 0.0f),
		Vector3f(-1.0f, 0.0f, 0.0f),
		Vector3f(0.0f, 1.0f, 0.0f),
		Vector3f(0.0f, -1.0f, 0.0f),
		Vector3f(0.0f, 0.0f, 1.0f),
		Vector3f(0.0f, 0.0f, -1.0f),
	};
	std::array<Vector3f, 6> upDirections =
	{
		Vector3f(0.0f, 1.0f, 0.0f),
		Vector3f(0.0f, 1.0f, 0.0f),
		Vector3f(0.0f, 0.0f, -1.0f),
		Vector3f(0.0f, 0.0f, 1.0f),
		Vector3f(0.0f, 1.0f, 0.0f),
		Vector3f(0.0f, 1.0f, 0.0f),
	};

	for (size_t i = 0; i < 6; ++i)
	{
		m_viewMatrices[i] = BuildViewMatrix(eyePosition, eyesDirections[i], upDirections[i]);
	}

	m_projectionMatrix = BuildProjectionMatrix();
}

Affine3f CubeMappingCamera::BuildViewMatrix(const Vector3f& eyePosition, const Vector3f& eyeDirection, const Vector3f& upDirection)
{
	auto zAxis = eyeDirection.normalized();

#if defined(USING_DIRECTX)
	zAxis = -zAxis;
#endif

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

Affine3f CubeMappingCamera::BuildProjectionMatrix()
{
	// XMStoreFloat4x4(&m_projectionMatrix, XMMatrixPerspectiveFovLH(XM_PI / 2.0f, 1.0f, 0.1f, 1000.0f));
	return Affine3f();
}
