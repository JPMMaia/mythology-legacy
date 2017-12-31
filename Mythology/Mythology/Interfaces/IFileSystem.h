#pragma once

#include <string>

namespace GameEngine
{
	class IFileSystem
	{
	public:
		virtual ~IFileSystem() = default;

	public:
		virtual std::wstring GetWorkingDirectory() const = 0;
	};
}
