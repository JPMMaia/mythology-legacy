#include "pch.h"
#include "CppUnitTest.h"
#include "Common/Events/Event.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MythologyTestProject
{
	TEST_CLASS(EventTests)
	{
	private:
		class TestClass
		{
		public:
			Common::Event<TestClass, string, void*, int> OnSumEvent;

		public:
			void RaiseEvent(int number)
			{
				OnSumEvent(this, number);
			}
		};

	private:
		TestClass m_testClass;
		int m_count = 0;

	public:

		TEST_METHOD(EventTest1)
		{
			m_testClass.OnSumEvent += {"First", this, &EventTests::OnTestEventRaised};
			Assert::AreEqual(0, m_count);

			m_testClass.RaiseEvent(1);
			Assert::AreEqual(1, m_count);

			m_testClass.OnSumEvent += {"Second", this, &EventTests::OnTestEventRaised};
			m_testClass.RaiseEvent(1);
			Assert::AreEqual(3, m_count);
			m_testClass.OnSumEvent -= "First";

			m_testClass.OnSumEvent += {"Third", this, &EventTests::OnTestEventRaised2};
			m_testClass.RaiseEvent(1);
			Assert::AreEqual(6, m_count);

			m_testClass.OnSumEvent -= "Third";
			m_testClass.RaiseEvent(1);
			Assert::AreEqual(7, m_count);

			m_testClass.OnSumEvent -= "Second";
			m_testClass.RaiseEvent(1);
			Assert::AreEqual(7, m_count);

			m_testClass.OnSumEvent -= "Second";
		}

	public:
		void OnTestEventRaised(void*, int number)
		{
			m_count += number;
		}
		void OnTestEventRaised2(void*, int number)
		{
			m_count += 2*number;
		}
	};
}