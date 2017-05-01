#include "stdafx.h"
#include "CppUnitTest.h"
#include "Common/Events/Delegate.h"

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

			void RaiseEvent(int number)
			{
				m_sumEvent(this, number);
			}

		public:
			Common::Delegate<void*, int>::PublicInterface& SumEvent()
			{
				return m_sumEvent.GetPublicInterface();
			}

		private:
			Common::Delegate<void*, int> m_sumEvent;
		};

	private:
		TestClass m_testClass;
		int m_count = 0;

	public:

		TEST_METHOD(EventTest1)
		{
			m_testClass.SumEvent() += Common::MemberCallback(*this, &EventTests::OnTestEventRaised);
			Assert::AreEqual(0, m_count);

			m_testClass.RaiseEvent(1);
			Assert::AreEqual(1, m_count);

			m_testClass.SumEvent() += Common::MemberCallback(*this, &EventTests::OnTestEventRaised);
			m_testClass.RaiseEvent(1);
			Assert::AreEqual(3, m_count);
			m_testClass.SumEvent() -= Common::MemberCallback(*this, &EventTests::OnTestEventRaised);

			m_testClass.SumEvent() += Common::MemberCallback(*this, &EventTests::OnTestEventRaised2);
			m_testClass.RaiseEvent(1);
			Assert::AreEqual(6, m_count);

			m_testClass.SumEvent() -= Common::MemberCallback(*this, &EventTests::OnTestEventRaised2);
			m_testClass.RaiseEvent(1);
			Assert::AreEqual(7, m_count);

			m_testClass.SumEvent() -= Common::MemberCallback(*this, &EventTests::OnTestEventRaised);
			m_testClass.RaiseEvent(1);
			Assert::AreEqual(7, m_count);

			m_testClass.SumEvent() -= Common::MemberCallback(*this, &EventTests::OnTestEventRaised);
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