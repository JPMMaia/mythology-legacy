#pragma once

#include <list>

namespace Events
{
	template<typename HandlerType>
	class Event
	{
	public:
		void Subscribe(HandlerType&& handler)
		{
			m_handlers.emplace(std::forward<HandlerType>(handler));
		}
		void Unsubscribe(HandlerType&& handler)
		{
			m_handlers.remove_if([&handler](const HandlerType& element) { return element == handler; });
		}

		template<typename... ArgumentsType>
		void Raise(ArgumentsType&&... arguments)
		{
			for (auto&& handler : m_handlers)
				handler(std::forward<ArgumentsType>(arguments)...);
		}

	private:
		std::list<HandlerType> m_handlers;
	};
}
