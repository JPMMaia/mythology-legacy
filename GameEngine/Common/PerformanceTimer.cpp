module Common.Timers;

import std.core;

export namespace Common
{
	export class PerformanceTimer
	{
	private:
		using ClockType = std::chrono::high_resolution_clock;
		using TimePointType = ClockType::time_point;

	public:
		void Start()
		{
			m_start = ClockType::now();
		}
		void End()
		{
			m_end = ClockType::now();
		}

		template <class Representation, class Period>
		std::chrono::duration<Representation, Period> ElapsedTime() const
		{
			return m_end - m_start;
		}

	private:
		TimePointType m_start;
		TimePointType m_end;
	};
}
