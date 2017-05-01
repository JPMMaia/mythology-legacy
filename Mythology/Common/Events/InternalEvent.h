#pragma once

namespace Common
{
	template<typename SenderType>
	class InternalEvent
	{
	public:
		virtual ~InternalEvent() = default;

		virtual void Handle(SenderType& sender) = 0;
	};
}