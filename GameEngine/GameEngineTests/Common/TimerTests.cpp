#include "ModuleTests/CppUnitTest.h"

import std.core;
import Common.Timers;

using namespace Common;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

long fibonacci(unsigned n)
{
	if (n < 2) return n;
	return fibonacci(n - 1) + fibonacci(n - 2);
}

void hello()
{

	auto start = std::chrono::system_clock::now();
	std::cout << "f(42) = " << fibonacci(42) << '\n';
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	auto t1 = std::chrono::system_clock::time_point::max();
	auto t2 = std::chrono::system_clock::now();

	auto delay = t2 - t1;
}

namespace GameEngineTests
{
	TEST_CLASS(TimerTests)
	{
	public:

		//	After a call to the Reset and UpdateAndRender methods, the total and delta time should be equal.
		TEST_METHOD(Reset)
		{
			auto processInput = []()
			{
				return true;
			};
			auto update = [](const Timer& timer)
			{
			};

			Timer timer;

			timer.Reset();
			timer.UpdateAndRender(update, update, update, processInput, update);
			Assert::IsTrue(timer.GetTotalTime() == timer.GetDeltaTime());

			timer.Reset();
			timer.UpdateAndRender(update, update, update, processInput, update);
			Assert::IsTrue(timer.GetTotalTime() == timer.GetDeltaTime());
		}

		// Successive calls to fixed update increase total time by a fixed ammount.
		TEST_METHOD(FixedUpdate)
		{
			auto fixedDeltaTime = std::chrono::microseconds(1);
			Timer timer(fixedDeltaTime);

			auto processInput = []()
			{
				return true;
			};
			auto other = [&](const Timer& timer)
			{
			};

			auto updateCount = 0;
			auto fixedUpdate = [&](const Timer& timer)
			{
				auto totalDeltaTime = fixedDeltaTime * updateCount;
				Assert::IsTrue(totalDeltaTime == timer.GetFixedUpdateTotalTime());
				
				Assert::IsTrue(fixedDeltaTime == timer.GetFixedUpdateDeltaTime());

				++updateCount;
			};

			size_t testCount = 100;
			for (size_t i = 0; i < testCount; ++i)
			{
				timer.UpdateAndRender(fixedUpdate, other, other, processInput, other);
			}
		}
	};
}