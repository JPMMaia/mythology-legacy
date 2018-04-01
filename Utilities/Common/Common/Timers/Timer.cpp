#include "pch.h"
#include "Timer.h"

#include <thread>

using namespace Common;
using namespace std;

Timer::Timer() :
	m_timePerUpdate(std::chrono::microseconds(12)),
	m_deltaTime(ClockType::duration(0))
{
}
Timer::Timer(DurationType timePerUpdate) :
	m_timePerUpdate(timePerUpdate),
	m_deltaTime(ClockType::duration(0))
{
}

void Timer::Reset()
{
	unique_lock<shared_mutex> lock(m_mutex);

	m_previousTimePoint = ClockType::now();
	m_lag = DurationType::zero();
}

void Timer::Update()
{
	unique_lock<shared_mutex> lock(m_mutex);

	m_currentTimePoint = ClockType::now();

	m_deltaTime = m_currentTimePoint - m_previousTimePoint;
	m_previousTimePoint = m_currentTimePoint;
	m_totalTime += m_deltaTime;

	m_lag += m_deltaTime;
}

void Timer::RunFixedUpdate(const std::function<void(const Timer&)>& fixedUpdate)
{
	std::size_t updateCount;

	{
		shared_lock<shared_mutex> lock(m_mutex);

		if (m_lag < m_timePerUpdate)
			return;

		updateCount = static_cast<std::size_t>(m_lag / m_timePerUpdate);
	}

	// Update the times needed to catchup:
	for (std::size_t i = 0; i < updateCount; ++i)
	{
		fixedUpdate(*this);
	}

	{
		unique_lock<shared_mutex> lock(m_mutex);
		m_lag -= m_timePerUpdate * updateCount;
	}
}

void Timer::SetTotalTime(DurationType totalTime)
{
	unique_lock<shared_mutex> lock(m_mutex);
	m_totalTime = totalTime;
}

void Common::Timer::SetDeltaTime(DurationType deltaTime)
{
	unique_lock<shared_mutex> lock(m_mutex);
	m_deltaTime = deltaTime;
}

Timer::DurationType Timer::GetTimePerUpdate() const
{
	shared_lock<shared_mutex> lock(m_mutex);
	return m_timePerUpdate;
}
Timer::DurationType Timer::GetTotalTime() const
{
	shared_lock<shared_mutex> lock(m_mutex);
	return m_totalTime;
}
Timer::DurationType Timer::GetDeltaTime() const
{
	shared_lock<shared_mutex> lock(m_mutex);
	return m_deltaTime;
}
double Timer::GetLeftover() const
{
	shared_lock<shared_mutex> lock(m_mutex);
	return std::chrono::duration<double>(m_lag) / std::chrono::duration<double>(m_timePerUpdate);
}
size_t Timer::GetFramesPerSecond() const
{
	shared_lock<shared_mutex> lock(m_mutex);
	return m_framesPerSecond;
}
Timer::DurationType Timer::GetTimePerFrame() const
{
	shared_lock<shared_mutex> lock(m_mutex);
	return m_timePerFrame;
}
