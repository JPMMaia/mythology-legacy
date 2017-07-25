#pragma once

#include <bitset>
#include "Common/Events/Event.h"

namespace DirectX12Engine
{
	class Keyboard
	{
	public:
		Common::Event<Keyboard, std::string, std::uint8_t> OnKeyDown;
		Common::Event<Keyboard, std::string, std::uint8_t> OnKeyUp;

	public:
		void KeyDown(std::uint8_t virtualKey);
		void KeyUp(std::uint8_t virtualKey);

		bool IsKeyDown(std::uint8_t virtualKey) const;

	private:
		std::bitset<256> m_keysState;
	};
}
