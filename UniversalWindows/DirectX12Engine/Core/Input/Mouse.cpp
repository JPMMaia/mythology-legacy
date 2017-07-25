#include "pch.h"
#include "Mouse.h"

using namespace DirectX;
using namespace DirectX12Engine;

Mouse::Mouse() :
m_position(XMFLOAT2(0.0f, 0.0f)),
m_lastPosition(m_position)

{
}

void Mouse::ProcessInput()
{
	if(m_isPositionDirty)
	{
		m_deltaPosition.x = m_position.x - m_lastPosition.x;
		m_deltaPosition.y = m_position.y - m_lastPosition.y;

		m_lastPosition = m_position;

		m_isPositionDirty = false;
	}
}

void Mouse::SetKeysState(bool left, bool middle, bool right)
{
	if (left && !m_keysState.test(0))
	{
		m_keysState.set(0);
		OnKeyPress(0);
	}
	else if (!left && m_keysState.test(0))
	{
		m_keysState.reset(0);
		OnKeyRelease(0);
	}

	if (middle && !m_keysState.test(1))
	{
		m_keysState.set(1);
		OnKeyPress(1);
	}
	else if (!middle && m_keysState.test(1))
	{
		m_keysState.reset(1);
		OnKeyRelease(1);
	}

	if (right && !m_keysState.test(2))
	{
		m_keysState.set(2);
		OnKeyPress(2);
	}
	else if (!right && m_keysState.test(2))
	{
		m_keysState.reset(2);
		OnKeyRelease(2);
	}
}

void Mouse::SetMousePosition(float x, float y)
{
	m_position.x = x;
	m_position.y = y;

	if(m_isPositionDirty)
	{
		m_lastPosition = m_position;
	}

	m_isPositionDirty = true;
}

const DirectX::XMFLOAT2& Mouse::DeltaPosition() const
{
	return m_deltaPosition;
}

bool Mouse::IsKeyDown(std::uint8_t key) const
{
	return m_keysState.test(key);
}
