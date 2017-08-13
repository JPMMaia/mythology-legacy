#pragma once

#include "Common/Events/Event.h"

#include <array>
#include <bitset>

namespace GameEngine
{
	class Mouse
	{
	public:
		Common::Event<Mouse, std::string, std::uint8_t> OnKeyPress;
		Common::Event<Mouse, std::string, std::uint8_t> OnKeyRelease;

	public:
		Mouse();

		void Update();

		void SetKeysState(bool left, bool middle, bool right);
		void ProcessMouseDelta(float x, float y);

		std::array<float, 2> DeltaMovement() const;

		bool IsKeyDown(std::uint8_t key) const;

	private:
		std::bitset<3> m_keysState;

		std::array<float, 2> m_position;
		std::array<float, 2> m_lastPosition;
		std::array<float, 2> m_deltaMovement;
		bool m_isDirty;
	};
}
