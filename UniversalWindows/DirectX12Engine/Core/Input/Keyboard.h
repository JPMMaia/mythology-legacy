#pragma once

#include <bitset>
#include "Common/Events/Event.h"

namespace DirectX12Engine
{
	class Keyboard
	{
	public:
		Common::Event<Keyboard, std::string, std::uint8_t> OnKeyPress;
		Common::Event<Keyboard, std::string, std::uint8_t> OnKeyRelease;

	public:
		void PressKey(std::uint8_t virtualKey);
		void ReleaseKey(std::uint8_t virtualKey);

		bool IsKeyDown(std::uint8_t virtualKey) const;

	private:
		std::bitset<256> m_keysState;
	};
}
