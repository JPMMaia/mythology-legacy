#include "pch.h"
#include "Ray.h"

using namespace DirectX;
using namespace DirectX12Engine;

Ray::Ray(FXMVECTOR origin, FXMVECTOR direction) :
	m_origin(origin),
	m_direction(direction)
{
}
Ray::Ray(const XMFLOAT3& origin, const XMFLOAT3& direction) :
	m_origin(XMLoadFloat3(&origin)),
	m_direction(XMVector3Normalize(XMLoadFloat3(&direction)))
{
}

XMVECTOR Ray::CalculatePoint(float distance) const
{
	return m_origin + m_direction * distance;
}