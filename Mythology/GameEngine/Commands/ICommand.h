#pragma once

namespace GameEngine
{
	class ICommand
	{
	public:
		virtual void Execute() = 0;
	};
}
