#pragma once

#include "Common/MemoryPool/MemoryPoolElement.h"

#include <deque>

namespace GameEngine
{
	template<typename Type>
	class MemoryPoolNode
	{
	public:
		using NextType = std::size_t;

	private:
		union State
		{
			Type Element;
			NextType Next;

			template<typename... ArgumentsType>
			explicit State(ArgumentsType&&... arguments) :
				Element(std::forward<ArgumentsType>(arguments)...)
			{
			}
			~State()
			{
			}
		};

	public:
		MemoryPoolNode() = default;
		~MemoryPoolNode() = default;

		template<typename... ArgumentsType>
		explicit MemoryPoolNode(ArgumentsType&&... arguments) :
			m_state(std::forward<ArgumentsType>(arguments)...),
			m_initialized(true)
		{
		}

		Type& Initialize()
		{
			m_initialized = true;
			return m_state.Element;
		}

		void Shutdown(NextType next)
		{
			m_state.Element.~Type();
			m_state.Next = next;
			m_initialized = false;
		}

		NextType GetNext() const
		{
			return m_state.Next;
		}
		Type& GetElement()
		{
			if (!m_initialized)
				throw std::logic_error("Node needs to be initialized!");

			return m_state.Element;
		}
		const Type& GetElement() const
		{
			if (!m_initialized)
				throw std::logic_error("Node needs to be initialized!");

			return m_state.Element;
		}

		void SetNext(NextType next)
		{
			m_state.Next = next;
		}

		bool IsInitialized() const
		{
			return m_initialized;
		}

	private:
		State m_state;
		bool m_initialized = false;
	};

	template <class T>
	class StandardAllocator
	{
	public:
		using value_type = T;
		using element_type = MemoryPoolNode<value_type>;

	public:
		StandardAllocator() = default;

		template <class U>
		constexpr StandardAllocator(const StandardAllocator<U>& other) noexcept
		{
		}

	public:

		value_type* allocate(std::size_t n)
		{
			// Emplace node at the end of the deque:
			s_storage.emplace_back();

			// Get emplaced node:
			auto& node = s_storage.back();

			// Initialize node and get pointer to element:
			return &node.Initialize();
		}
		void deallocate(value_type* p, std::size_t n) noexcept
		{
			// Find node:
			auto location = std::find_if(s_storage.begin(), s_storage.end(), [p](auto const& o)
			{
				return &o.GetElement() == p;
			});

			// Shutdown node:
			location->Shutdown(0);

			// Remove all nodes at the end of the deque that are not initialized:
			auto it = s_storage.rbegin();
			if(it->IsInitialized())
			{
				
			}
			//while(!->IsInitialized())
				//s_storage.erase(s_storage.rbegin());
		}

		std::size_t size() const
		{
			return s_storage.size();
		}

	private:
		static std::deque<element_type> s_storage;
		std::size_t m_firstAvailable = -1;
	};

	template<class T>
	std::deque<typename StandardAllocator<T>::element_type> StandardAllocator<T>::s_storage;

	template <class T, class U>
	bool operator==(const StandardAllocator<T>& a, const StandardAllocator<U>& b)
	{
		return true;
	}

	template <class T, class U>
	bool operator!=(const StandardAllocator<T>& a, const StandardAllocator<U>& b)
	{
		return !(a == b);
	}
}
