#include "pch.h"
#include "Keyboard.h"

using namespace DirectX12Engine;

void Keyboard::KeyDown(std::uint8_t virtualKey)
{
	m_keysState.set(static_cast<std::size_t>(virtualKey));

	OnKeyDown(virtualKey);
}
void Keyboard::KeyUp(std::uint8_t virtualKey)
{
	m_keysState.reset(static_cast<std::size_t>(virtualKey));

	OnKeyUp(virtualKey);
}

bool Keyboard::IsKeyDown(std::uint8_t virtualKey) const
{
	return m_keysState.test(static_cast<std::size_t>(virtualKey));
}
