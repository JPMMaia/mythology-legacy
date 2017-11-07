#include "pch.h"
#include "CubeMappingCamera.h"

using namespace Eigen;
using namespace GameEngine;

CubeMappingCamera::CubeMappingCamera(const Eigen::Vector3f& eyePosition)
{
	BuildMatrices(eyePosition);
}

const Eigen::Affine3f& CubeMappingCamera::GetViewMatrix(std::size_t index) const
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

	for (std::size_t i = 0; i < 6; ++i)
	{
		m_viewMatrices[i] = BuildViewMatrix(eyePosition, eyesDirections[i], upDirections[i]);
	}

	auto fovAngleY = std::acos(-1.0f) / 2.0f;
	auto aspectRatio = 1.0f;
	auto nearZ = 0.1f;
	auto farZ = 15.0f;
	m_projectionMatrix = BuildProjectionMatrix(fovAngleY, aspectRatio, nearZ, farZ);
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

Affine3f CubeMappingCamera::BuildProjectionMatrix(float fovAngleY, float aspectRatio, float nearZ, float farZ)
{	
	auto yScale = 1.0f / std::tan(fovAngleY / 2.0f);
	auto xScale = yScale / aspectRatio;

#if defined(USING_DIRECTX)
	auto nearDepth = 0.0f;
#else
	auto nearDepth = -1.0f;
#endif
	auto farDepth = 1.0f;

	auto perspectiveMatrix(Affine3f::Identity());
	perspectiveMatrix(0, 0) = xScale;
	perspectiveMatrix(1, 1) = yScale;
	perspectiveMatrix(2, 2) = (farDepth * farZ - nearDepth * nearZ) / (nearZ - farZ);
	perspectiveMatrix(3, 2) = -1.0f;
	perspectiveMatrix(2, 3) = (farDepth - nearDepth) * (nearZ * farZ) / (nearZ - farZ);
	perspectiveMatrix(3, 3) = 0.0f;

	return perspectiveMatrix;
}
