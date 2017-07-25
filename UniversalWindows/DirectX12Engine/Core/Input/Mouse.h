#pragma once

#include "Common/Events/Event.h"

#include <bitset>

namespace DirectX12Engine
{
	class Mouse
	{
	public:
		Common::Event<Mouse, std::string, std::uint8_t> OnKeyPress;
		Common::Event<Mouse, std::string, std::uint8_t> OnKeyRelease;

	public:
		void SetKeysState(bool left, bool middle, bool right);
		void SetMousePosition(float x, float y);

		bool IsKeyDown(std::uint8_t key) const;

	private:
		std::bitset<3> m_keysState;
		DirectX::XMFLOAT2 m_position;
	};
}
