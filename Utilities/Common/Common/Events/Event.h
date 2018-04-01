#pragma once

#include <functional>
#include <unordered_map>

namespace Common
{
	template <typename OwnerType, typename KeyType, typename... ParametersTypes>
	class Event
	{
		friend OwnerType;

	public:
		using FunctionType = std::function<void(ParametersTypes...)>;

		struct Binding
		{
			KeyType Key;
			FunctionType Function;

			Binding(KeyType key, FunctionType function)
			{
				this->Key = key;
				this->Function = function;
			}

			template<typename TargetType>
			Binding(KeyType key, TargetType* target, void(TargetType::*function)(ParametersTypes...))
			{
				this->Key = key;
				this->Function = [target, function](ParametersTypes... args)
				{
					return (target->*function)(args...);
				};
			}
		};

	private:
		Event() = default;
		Event(const Event& from) = default;
		Event(Event&& from) = default;
		Event& operator=(const Event& from) = default;
		Event& operator=(Event&& from) = default;
		~Event() = default;

	public:
		Event& operator +=(const Binding& binding)
		{
			m_bindings.emplace(binding.Key, binding.Function);
			return *this;
		}
		Event& operator -=(KeyType key)
		{
			m_bindings.erase(key);
			return *this;
		}

	private:
		void operator()(ParametersTypes... arguments) const
		{
			auto localBindingsCopy = m_bindings;
			for (const auto& binding : localBindingsCopy)
			{
				binding.second(arguments...);
			}
		}

	private:
		std::unordered_map<KeyType, FunctionType> m_bindings;
	};
}
