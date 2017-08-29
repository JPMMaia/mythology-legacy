#include "pch.h"
#include "PerformanceTimer.h"

using namespace Common;

void PerformanceTimer::Start()
{
	m_start = ClockType::now();
}

void PerformanceTimer::End()
{
	m_end = ClockType::now();
}
