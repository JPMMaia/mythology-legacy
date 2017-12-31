#pragma once

#include <chrono>
#include <functional>
#include <shared_mutex>

namespace Common
{
	class Timer
	{
	public:
		using ClockType = std::chrono::high_resolution_clock;
		using TimePointType = ClockType::time_point;
		using DurationType = ClockType::duration;

	public:
		Timer();
		explicit Timer(DurationType timePerUpdate);

		void Reset();

		void Update();

		void RunFixedUpdate(const std::function<void(const Timer&)>& fixedUpdate);

		template <typename FixedUpdateFunctionType, typename FrameUpdateFunctionType, typename RenderFunctionType, typename ProcessInputFunctionType, typename ProcessFrameStatisticsFunctionType>
		bool UpdateAndRender(FixedUpdateFunctionType&& fixedUpdate, FrameUpdateFunctionType&& frameUpdate, RenderFunctionType&& render, ProcessInputFunctionType&& processInput, ProcessFrameStatisticsFunctionType&& processFrameStatistics)
		{
			Update();

			// Process input:
			processInput();

			// Update the times needed to catchup:
			while (m_lag >= m_timePerUpdate)
			{
				fixedUpdate(*this);
				m_lag -= m_timePerUpdate;
			}

			// Render:
			if (!frameUpdate(*this))
				return false;
			render(*this);

			// Calculate frames statistics (frames per second, milliseconds per frame):
			CalculateFrameStatistics(processFrameStatistics);

			return true;
		}

		DurationType GetTimePerUpdate() const;
		DurationType GetTotalTime() const;
		DurationType GetDeltaTime() const;
		double GetLeftover() const;
		size_t GetFramesPerSecond() const;
		DurationType GetTimePerFrame() const;

		void SetTotalTime(DurationType totalTime);
		void SetDeltaTime(DurationType deltaTime);

	private:
		template<typename ProcessFrameStatisticsFunctionType>
		void CalculateFrameStatistics(ProcessFrameStatisticsFunctionType&& processFrameStatistics);

	private:
		const DurationType m_timePerUpdate;

		DurationType m_totalTime;
		DurationType m_deltaTime;
		DurationType m_lag;

		TimePointType m_currentTimePoint;
		TimePointType m_previousTimePoint;

		size_t m_framesPerSecond = 0;
		DurationType m_timePerFrame;

		mutable std::shared_mutex m_mutex;
	};

	template <typename ProcessFrameStatisticsFunctionType>
	void Timer::CalculateFrameStatistics(ProcessFrameStatisticsFunctionType&& processFrameStatistics)
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
}
