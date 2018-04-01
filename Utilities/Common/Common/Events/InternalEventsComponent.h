#pragma once

#include <deque>
#include <mutex>

#include "InternalEvent.h"

namespace Common
{
	template<typename SenderType>
	class InternalEventsComponent
	{
	public:
		void HandleEvents(SenderType& sender)
		{
			std::lock_guard<std::mutex> lock(m_eventsMutex);

			while(!m_events.empty())
			{
				m_events.front()->Handle(sender);
				m_events.pop_front();
			}
		}

		void AddEvent(std::unique_ptr<InternalEvent<SenderType>>&& event)
		{
			std::lock_guard<std::mutex> lock(m_eventsMutex);
			m_events.emplace_back(std::move(event));
		}

	private:
		std::mutex m_eventsMutex;
		std::deque<std::unique_ptr<InternalEvent<SenderType>>> m_events;
	};
}
