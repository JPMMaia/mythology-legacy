#include "pch.h"
#include "Mouse.h"

using namespace DirectX12Engine;

Mouse::Mouse() :
	m_position({0.0f, 0.0f}),
	m_lastPosition({ 0.0f, 0.0f }),
	m_deltaMovement({0.0f, 0.0f}),
	m_isDirty(true)
{
}

void Mouse::Update()
{
	if(m_isDirty)
	{
		m_deltaMovement = { m_position[0] - m_lastPosition[0], m_position[1] - m_lastPosition[1] };

		m_lastPosition = m_position;
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

void Mouse::ProcessMouseDelta(float x, float y)
{
	m_position[0] += x;
	m_position[1] += y;
	m_isDirty = true;
}

std::array<float, 2> Mouse::DeltaMovement() const
{
	return m_deltaMovement;
}

bool Mouse::IsKeyDown(std::uint8_t key) const
{
	return m_keysState.test(key);
}
