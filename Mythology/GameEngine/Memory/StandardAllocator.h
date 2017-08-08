#pragma once

#include <cstdlib>
#include <deque>

namespace GameEngine
{
	template<typename Type>
	class MemoryPoolNode
	{
	public:
		using NodeType = MemoryPoolNode<Type>;

	private:
		union State
		{
			Type Element;
			struct
			{
				NodeType* Next;
				NodeType* Previous;
			} Meta;

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

		void Shutdown()
		{
			if (!m_initialized)
				return;

			m_state.Element.~Type();
			m_initialized = false;
		}

		Type* GetElement()
		{
			return &m_state.Element;
		}
		const Type* GetElement() const
		{
			return &m_state.Element;
		}

		NodeType* GetPrevious() const
		{
			return m_state.Meta.Previous;
		}
		void SetPrevious(NodeType* previous)
		{
			m_state.Meta.Previous = previous;
		}

		NodeType* GetNext() const
		{
			return m_state.Meta.Next;
		}
		void SetNext(NodeType* next)
		{
			m_state.Meta.Next = next;
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
			auto node = PopFront();

			// If a node is not available:
			if(!node)
			{
				// Emplace node at the end of the deque:
				s_storage.emplace_back();

				// Get emplaced node:
				node = &s_storage.back();
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
			node->Shutdown();

			// If node is not at the end, set as the front node:
			if(std::distance(node, s_storage.end()) > 1)
			{
				PushFront(&(*node));
			}

			// If node is at the end:
			else
			{
				// Remove back node:
				s_storage.pop_back();

				// Remove all nodes at the end of the deque that are not initialized:
				for(auto iterator = s_storage.rbegin(); iterator != s_storage.rend() && !iterator->IsInitialized(); iterator = s_storage.rbegin())
				{
					// Remove node from available nodes:
					RemoveAt(iterator);

					// Decrease storage:
					s_storage.pop_back();
				}
			}
		}

		static std::size_t size()
		{
			return s_storage.size();
		}

	private:
		static void PushFront(node_type* node)
		{
			if(node)
			{
				// Set previous to null:
				node->SetPrevious(nullptr);

				// Set the front node as the next of the pushed node:
				node->SetNext(s_front);
			}

			if(s_front)
			{
				// Set the previous node of the old front:
				s_front->SetPrevious(node);
			}

			// Set the pushed node as the front:
			s_front = node;
		}
		static node_type* PopFront()
		{
			node_type* node = nullptr;

			// If a node is available:
			if(s_front)
			{
				// Get node at the front:
				node = s_front;

				// Assign new front:
				s_front = s_front->GetNext();

				// Set previous to null:
				if(s_front)
					s_front->SetPrevious(nullptr);
			}

			return node;
		}
		static void RemoveAt(typename std::deque<node_type>::reverse_iterator location)
		{
			// Get previous node:
			auto previous = location->GetPrevious();

			// Get next node:
			auto next = location->GetNext();

			if(previous)
				previous->SetNext(next);

			if (next)
				next->SetPrevious(previous);
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