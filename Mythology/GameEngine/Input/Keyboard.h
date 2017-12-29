#pragma once

#include "Common/Events/Event.h"

#include <bitset>

namespace GameEngine
{
	class Keyboard
	{
	public:
		Common::Event<Keyboard, std::string, std::uint8_t> OnKeyPress;
		Common::Event<Keyboard, std::string, std::uint8_t> OnKeyRelease;

	public:
		Keyboard() = default;
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator=(const Keyboard&) = delete;
		Keyboard(Keyboard&&) = default;
		Keyboard& operator=(Keyboard&&) = default;

	public:
		void PressKey(std::uint8_t virtualKey);
		void ReleaseKey(std::uint8_t virtualKey);

		bool IsKeyDown(std::uint8_t virtualKey) const;

	private:
		std::bitset<256> m_keysState;
	};
}
