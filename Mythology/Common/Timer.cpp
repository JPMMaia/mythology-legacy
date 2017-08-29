#include "Timer.h"

using namespace Common;

Timer::Timer(DurationType timePerUpdate) :
	m_timePerUpdate(timePerUpdate),
	m_deltaTime(ClockType::duration(0))
{
}

void Timer::Reset()
{
	m_previousTimePoint = ClockType::now();
	m_lag = DurationType::zero();
}

void Timer::SetTotalTime(DurationType totalTime)
{
	m_totalTime = totalTime;
}

void Common::Timer::SetDeltaTime(DurationType deltaTime)
{
	m_deltaTime = deltaTime;
}

Timer::DurationType Timer::GetTimePerUpdate() const
{
	return m_timePerUpdate;
}
Timer::DurationType Timer::GetTotalTime() const
{
	return m_totalTime;
}
Timer::DurationType Timer::GetDeltaTime() const
{
	return m_deltaTime;
}
double Timer::GetLeftover() const
{
	return std::chrono::duration<double>(m_lag) / std::chrono::duration<double>(m_timePerUpdate);
}
size_t Timer::GetFramesPerSecond() const
{
	return m_framesPerSecond;
}
Timer::DurationType Timer::GetTimePerFrame() const
{
	return m_timePerFrame;
}
