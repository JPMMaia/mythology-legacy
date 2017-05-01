#pragma once

#include <algorithm>
#include <memory>
#include <list>

namespace Common
{
	template <typename... Args>
	class Delegate
	{
	public:
		using FunctionSignature = void(Args...);

	private:
		class Base
		{
		public:
			virtual ~Base()
			{
			}

		public:
			virtual bool Compare(Base* other) = 0;
			virtual void Raise(Args... args) = 0;
		};

		template <typename Type>
		class Callback : public Base
		{
		public:
			template <typename S>
			explicit Callback(S&& callback) :
				m_callback(std::forward<S>(callback))
			{
			}

			bool Compare(Base* other) override
			{
				Callback<Type>* tmp = dynamic_cast<Callback<Type>*>(other);
				return tmp && m_callback == tmp->m_callback;
			}
			void Raise(Args... args) override
			{
				return m_callback(std::forward<Args>(args)...);
			}

		private:
			Type m_callback;
		};

	public:
		class PublicInterface
		{
		public:
			explicit PublicInterface(Delegate& parent) :
				m_callbacks(parent.m_callbacks)
			{
			}

		public:
			template <typename Type>
			PublicInterface& operator+= (Type&& callback)
			{
				m_callbacks.emplace_back(std::make_unique<Callback<Type>>(std::forward<Type>(callback)));
				return *this;
			}

			template <typename Type>
			PublicInterface& operator-= (Type&& callback)
			{
				Callback<Type> tmp(std::forward<Type>(callback));

				auto comparer = [&](std::unique_ptr<Base>& other)
				{
					return tmp.Compare(other.get());
				};

				auto location = std::find_if(m_callbacks.begin(), m_callbacks.end(), comparer);
				if (location != m_callbacks.end())
					m_callbacks.erase(location);

				return *this;
			}

		private:
			std::list<std::unique_ptr<Base>>& m_callbacks;
		};

	public:
		Delegate() :
			m_publicInterface(*this)
		{
		}
		Delegate(const Delegate&) = delete;
		Delegate& operator=(const Delegate&) = delete;

		template <typename Type>
		Delegate& operator+= (Type&& callback)
		{
			m_publicInterface += std::forward<Type>(callback);
			return *this;
		}

		template <typename Type>
		Delegate& operator-= (Type&& callback)
		{
			m_publicInterface -= std::forward<Type>(callback);
			return *this;
		}

		void operator()(Args... args)
		{
			for (auto& callback : m_callbacks)
				callback->Raise(args...);
		}

		PublicInterface& GetPublicInterface()
		{
			return m_publicInterface;
		}

	private:
		std::list<std::unique_ptr<Base>> m_callbacks;
		PublicInterface m_publicInterface;
	};

	template <typename RC, typename Class, typename... Args>
	class MemberCallbackAux
	{
	public:
		MemberCallbackAux(Class* object, RC(Class::*member)(Args...)) :
			m_object(object),
			m_member(member)
		{
		}

		RC operator()(Args... args)
		{
			return (m_object->*m_member)(std::forward<Args>(args)...);
		}

		bool operator== (MemberCallbackAux const& other) const
		{
			return m_object == other.m_object
				&& m_member == other.m_member;
		}
		bool operator!= (MemberCallbackAux const& other) const
		{
			return !(*this == other);
		}

	private:
		Class* m_object;
		RC(Class::*m_member)(Args...);
	};

	template <typename RC, typename Class, typename... Args>
	MemberCallbackAux<RC, Class, Args...> MemberCallback(Class& object, RC(Class::*member)(Args...))
	{
		return MemberCallbackAux<RC, Class, Args...>(&object, member);
	}
}

#define DefineEvent(DelegateType, EventName)					\
public:															\
	DelegateType ## ::PublicInterface& SumEvent()					\
	{															\
		return e_SumEvent.GetPublicInterface();				\
	}															\
																\
private:														\
	Common::Delegate<void*, int> e_SumEvent;
