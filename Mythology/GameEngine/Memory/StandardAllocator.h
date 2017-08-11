#pragma once

#include <deque>

namespace GameEngine
{
	class CameraComponent;

	template<typename Type>
	class StandardAllocatorNode
	{
	public:
		using NodeType = StandardAllocatorNode<Type>;

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
		StandardAllocatorNode() = default;
		~StandardAllocatorNode() = default;

		template<typename... ArgumentsType>
		explicit StandardAllocatorNode(ArgumentsType&&... arguments) :
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

	template<typename T>
	class StandardAllocatorIterator
	{
	public:
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using reference = T&;
		using pointer = T*;
		using iterator_category = std::forward_iterator_tag;

	private:
		using node_type = StandardAllocatorNode<value_type>;

	public:
		StandardAllocatorIterator() = default;
		~StandardAllocatorIterator() = default;
		StandardAllocatorIterator(const StandardAllocatorIterator& other) = default;
		StandardAllocatorIterator(StandardAllocatorIterator&& other) noexcept = default;
		StandardAllocatorIterator& operator=(const StandardAllocatorIterator& other) = default;
		StandardAllocatorIterator& operator=(StandardAllocatorIterator&& other) noexcept = default;

		StandardAllocatorIterator(const typename std::deque<StandardAllocatorNode<T>>::iterator& begin, const typename std::deque<StandardAllocatorNode<T>>::iterator& end)
			: m_location(begin),
			  m_end(end)
		{
		}

	public:

		bool operator==(const StandardAllocatorIterator& other)
		{
			return m_location == other.m_location;
		}
		bool operator!=(const StandardAllocatorIterator& other)
		{
			return !(*this == other);
		}

		reference operator*()
		{
			return *m_location->GetElement();
		}
		pointer operator->()
		{
			return m_location->GetElement();
		}
		StandardAllocatorIterator& operator++()
		{
			do
			{
				++m_location;
			}
			while (m_location != m_end && !m_location->IsInitialized());

			return *this;
		}
		StandardAllocatorIterator operator++(int)
		{
			StandardAllocatorIterator other(*this);
			operator++();
			return other;
		}

	private:
		typename std::deque<node_type>::iterator m_location;
		typename std::deque<node_type>::iterator m_end;
	};

	template <class T>
	class StandardAllocator
	{
	public:
		using value_type = T;
		using iterator = StandardAllocatorIterator<value_type>;

	private:
		using node_type = StandardAllocatorNode<value_type>;

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
			if (!node)
			{
				// Emplace node at the end of the deque:
				m_storage.emplace_back();

				// Get emplaced node:
				node = &m_storage.back();
			}

			// Signal that node will be used to contain data:
			node->Initialize();

			// Return pointer to element:
			return node->GetElement();
		}
		void deallocate(value_type* p, std::size_t n) noexcept
		{
			// Find node:
			auto node = std::find_if(m_storage.begin(), m_storage.end(), [p](auto const& o)
			{
				return o.GetElement() == p;
			});
			assert(node != m_storage.end());

			// Signal that the node will be used to store a reference to the next node:
			node->Shutdown();

			// If node is not at the end, set as the front node:
			if (std::distance(node, m_storage.end()) > 1)
			{
				PushFront(&(*node));
			}

			// If node is at the end:
			else
			{
				// Remove back node:
				m_storage.pop_back();

				// Remove all nodes at the end of the deque that are not initialized:
				for (auto iterator = m_storage.rbegin(); iterator != m_storage.rend() && !iterator->IsInitialized(); iterator = m_storage.rbegin())
				{
					// Remove node from available nodes:
					RemoveAt(iterator);

					// Decrease storage:
					m_storage.pop_back();
				}
			}
		}

		iterator begin()
		{
			return iterator(m_storage.begin(), m_storage.end());
		}
		iterator end()
		{
			return iterator(m_storage.end(), m_storage.end());
		}

		std::size_t size() const
		{
			return m_storage.size();
		}

	private:
		void PushFront(node_type* node)
		{
			if (node)
			{
				// Set previous to null:
				node->SetPrevious(nullptr);

				// Set the front node as the next of the pushed node:
				node->SetNext(m_front);
			}

			if (m_front)
			{
				// Set the previous node of the old front:
				m_front->SetPrevious(node);
			}

			// Set the pushed node as the front:
			m_front = node;
		}
		node_type* PopFront()
		{
			node_type* node = nullptr;

			// If a node is available:
			if (m_front)
			{
				// Get node at the front:
				node = m_front;

				// Assign new front:
				m_front = m_front->GetNext();

				// Set previous to null:
				if (m_front)
					m_front->SetPrevious(nullptr);
			}

			return node;
		}
		void RemoveAt(typename std::deque<node_type>::reverse_iterator location)
		{
			// Get previous node:
			auto previous = location->GetPrevious();

			// Get next node:
			auto next = location->GetNext();

			if (previous)
				previous->SetNext(next);

			if (next)
				next->SetPrevious(previous);
		}

	private:
		std::deque<node_type> m_storage;
		node_type* m_front = nullptr;
	};

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
