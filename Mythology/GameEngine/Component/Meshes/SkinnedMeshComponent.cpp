#include "SkinnedMeshComponent.h"

using namespace GameEngine;

SkinnedMeshComponent::SkinnedMeshComponent(const std::string & name) :
	m_name(name)
{
}

const std::vector<CustomGeometry<EigenMeshData>>& SkinnedMeshComponent::GetGeometries() const
{
	return m_geometries;
}
