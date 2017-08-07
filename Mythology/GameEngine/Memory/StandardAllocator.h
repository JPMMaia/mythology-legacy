#pragma once

#include <cstdlib>

#include "Common/MemoryPool/Allocator.h"
#include <deque>

namespace GameEngine
{
	/*template <class T>
	class StandardAllocator
	{
	public:
		typedef T value_type;

	public:
		StandardAllocator() = default;

		template <class U>
		constexpr StandardAllocator(const StandardAllocator<U>&) noexcept
		{
		}

	public:
		T* allocate(std::size_t size)
		{
			return reinterpret_cast<T*>(s_allocator.Allocate(size));
		}
		void deallocate(T* pBlock, std::size_t) noexcept
		{
			s_allocator.Deallocate(pBlock);
		}

	private: \
		static Common::Allocator s_allocator;
		//static Common::AllocatorPool<T, 2> s_allocator;
	};

	template<class T>
	Common::Allocator StandardAllocator<T>::s_allocator(sizeof(StandardAllocator<T>), 0, nullptr, "StandardAllocator");

	//template<class T>
	//Common::AllocatorPool<T, 2> StandardAllocator<T>::s_allocator;

	template <class T, class U>
	bool operator==(const StandardAllocator<T>&, const StandardAllocator<U>&) { return true; }
	template <class T, class U>
	bool operator!=(const StandardAllocator<T>&, const StandardAllocator<U>&) { return false; }*/

	template<typename Type>
	class MemoryPoolNode
	{
	public:
		using NodeType = MemoryPoolNode<Type>;

	private:
		union State
		{
			Type Element;
			NodeType* Next;

			State()
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

		void Initialize()
		{
			m_initialized = true;
		}

		void Shutdown(NodeType* next)
		{
			m_state.Element.~Type();
			m_state.Next = next;
			m_initialized = false;
		}

		NodeType* GetNext() const
		{
			return m_state.Next;
		}
		Type* GetElement()
		{
			return &m_state.Element;
		}
		const Type* GetElement() const
		{
			return &m_state.Element;
		}

		void SetNext(NodeType* next)
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
		using node_type = MemoryPoolNode<value_type>;

	public:
		StandardAllocator() = default;
		~StandardAllocator()
		{
		}

		template <class U>
		constexpr StandardAllocator(const StandardAllocator<U>& other) noexcept
		{
		}

	public:

		value_type* allocate(std::size_t n)
		{
			// Try to get the front node:
			auto node = Pop();

			// If a node is not available:
			if(!node)
			{
				// Emplace node at the end of the deque:
				s_storage.emplace_back();

				// Get emplaced node:
				node = &s_storage.back();

				// Set as back node:
				node->SetNext(nullptr);

				// If storage wasn't empty:
				if(s_storage.size() > 1)
				{
					// Append node to the queue:
					(++s_storage.rbegin())->SetNext(node);
				}
			}

			// Signal that node will be used to contain data:
			node->Initialize();

			// Return pointer to element:
			return node->GetElement();
		}
		void deallocate(value_type* p, std::size_t n) noexcept
		{
			// Find node:
			auto node = std::find_if(s_storage.begin(), s_storage.end(), [p](auto const& o)
			{
				return o.GetElement() == p;
			});
			assert(node != s_storage.end());

			// Signal that the node will be used to store a reference to the next node:
			node->Shutdown(0);

			// If at middle, set as the front node:
			if(node != s_storage.rbegin())
				Push(&(*node));

			// Remove all nodes at the end of the deque that are not initialized:
			while(!s_storage.rbegin()->IsInitialized())
				s_storage.pop_back();
		}

		static std::size_t size()
		{
			return s_storage.size();
		}

	private:
		static void Push(node_type* node)
		{
			// Set the front node as the next of the pushed node:
			node->SetNext(s_front);

			// Set the pushed node as the head:
			s_front = node;
		}
		static node_type* Pop()
		{
			node_type* node = nullptr;

			// If a node is available:
			if(s_front)
			{
				// Get node at the front:
				node = s_front;

				// Assign new front:
				s_front = s_storage.at(s_front).GetNext();
			}

			return node;
		}

	private:
		static std::deque<node_type> s_storage;
		static node_type* s_front;
	};

	template<class T>
	std::deque<typename StandardAllocator<T>::node_type> StandardAllocator<T>::s_storage;

	template<class T>
	typename StandardAllocator<T>::node_type* StandardAllocator<T>::s_front;

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