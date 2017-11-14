module Common.Timers;

import std.core;

export namespace Common
{
	export class Timer
	{
	public:
		using ClockType = std::chrono::high_resolution_clock;
		using TimePointType = typename ClockType::time_point;
		using DurationType = typename ClockType::duration;

	public:
		Timer() :
			m_timePerUpdate(std::chrono::milliseconds(12))
		{
			Reset();
		}
		explicit Timer(DurationType timePerUpdate) :
			m_timePerUpdate(timePerUpdate)
		{
			Reset();
		}

		void Reset()
		{
			m_deltaTime = ClockType::duration(0);
			m_fixedUpdateCount = 0;
			m_previousTimePoint = ClockType::now();
			m_lag = DurationType::zero();
			m_totalTime = DurationType::zero();
		}

		template <typename FixedUpdateFunctionType, typename FrameUpdateFunctionType, typename RenderFunctionType, typename ProcessInputFunctionType, typename ProcessFrameStatisticsFunctionType>
		bool UpdateAndRender(FixedUpdateFunctionType&& fixedUpdate, FrameUpdateFunctionType&& frameUpdate, RenderFunctionType&& render, ProcessInputFunctionType&& processInput, ProcessFrameStatisticsFunctionType&& processFrameStatistics)
		{
			m_currentTimePoint = ClockType::now();

			m_deltaTime = m_currentTimePoint - m_previousTimePoint;
			m_previousTimePoint = m_currentTimePoint;
			m_totalTime += m_deltaTime;

			m_lag += m_deltaTime;

			// Process input:
			if (!processInput())
				return false;

			// Update the times needed to catchup:
			while (m_lag >= m_timePerUpdate)
			{
				fixedUpdate(*this);
				++m_fixedUpdateCount;
				m_lag -= m_timePerUpdate;
			}

			// Render:
			frameUpdate(*this);
			render(*this);

			// Calculate frames statistics (frames per second, milliseconds per frame):
			CalculateFrameStatistics(processFrameStatistics);

			return true;
		}

		DurationType GetFixedUpdateDeltaTime() const
		{
			return m_timePerUpdate;
		}
		DurationType GetFixedUpdateTotalTime() const
		{
			return m_timePerUpdate * m_fixedUpdateCount;
		}
		DurationType GetTotalTime() const
		{
			return m_totalTime;
		}
		DurationType GetDeltaTime() const
		{
			return m_deltaTime;
		}
		double GetLeftover() const
		{
			return std::chrono::duration<double>(m_lag) / std::chrono::duration<double>(m_timePerUpdate);
		}
		size_t GetFramesPerSecond() const
		{
			return m_framesPerSecond;
		}
		DurationType GetTimePerFrame() const
		{
			return m_timePerFrame;
		}

		void SetTotalTime(DurationType totalTime)
		{
			m_totalTime = totalTime;
		}
		void SetDeltaTime(DurationType deltaTime)
		{
			m_deltaTime = deltaTime;
		}

	private:
		template<typename ProcessFrameStatisticsFunctionType>
		void CalculateFrameStatistics(ProcessFrameStatisticsFunctionType&& processFrameStatistics)
		{
			static auto elapsedTime = DurationType::zero();
			static size_t frameCount = 0;

			// Increment frame count:
			++frameCount;

			// If one second has elapsed:
			if (m_totalTime - elapsedTime >= std::chrono::seconds(1))
			{
				// Update values:
				m_framesPerSecond = frameCount;
				m_timePerFrame = std::chrono::seconds(1) / static_cast<std::chrono::seconds::rep>(frameCount);

				// Reset frame count and increment elapsed time by 1 second:
				frameCount = 0;
				elapsedTime += std::chrono::seconds(1);

				processFrameStatistics(*this);
			}
		}

	private:
		const DurationType m_timePerUpdate;
		std::size_t m_fixedUpdateCount = 0;

		DurationType m_totalTime;
		DurationType m_deltaTime;
		DurationType m_lag;

		TimePointType m_currentTimePoint;
		TimePointType m_previousTimePoint;

		std::size_t m_framesPerSecond = 0;
		DurationType m_timePerFrame;
	};
}
