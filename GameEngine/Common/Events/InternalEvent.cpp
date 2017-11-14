module Common.Events;

export namespace Common
{
	export template<typename SenderType>
	class InternalEvent
	{
	public:
		virtual ~InternalEvent()
		{
		}

		virtual void Handle(SenderType& sender) = 0;
	};
}