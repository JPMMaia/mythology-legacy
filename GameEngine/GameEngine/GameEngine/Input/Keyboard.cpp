#include "pch.h"
#include "Keyboard.h"

using namespace GameEngine;

void Keyboard::PressKey(std::uint8_t virtualKey)
{
	m_keysState.set(static_cast<std::size_t>(virtualKey));

	OnKeyPress(virtualKey);
}
void Keyboard::ReleaseKey(std::uint8_t virtualKey)
{
	m_keysState.reset(static_cast<std::size_t>(virtualKey));

	OnKeyRelease(virtualKey);
}

bool Keyboard::IsKeyDown(std::uint8_t virtualKey) const
{
	return m_keysState.test(static_cast<std::size_t>(virtualKey));
}
