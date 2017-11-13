#pragma once

namespace GameEngine
{
	class CubeMappingCamera
	{
	public:
		CubeMappingCamera() = default;
		CubeMappingCamera(const Eigen::Vector3f& eyePosition);

		const Eigen::Affine3f& GetViewMatrix(std::size_t index) const;
		const Eigen::Affine3f& GetProjectionMatrix() const;

	private:
		void BuildMatrices(const Eigen::Vector3f& eyePosition);
		Eigen::Affine3f BuildViewMatrix(const Eigen::Vector3f& eyePosition, const Eigen::Vector3f& eyeDirection, const Eigen::Vector3f& upDirection);
		Eigen::Affine3f BuildProjectionMatrix(float fovAngleY, float aspectRatio, float nearZ, float farZ);

	private:
		std::array<Eigen::Affine3f, 6> m_viewMatrices;
		Eigen::Affine3f m_projectionMatrix;
	};
}