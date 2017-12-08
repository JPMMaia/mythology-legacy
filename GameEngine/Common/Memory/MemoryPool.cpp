export module Common.MemoryPools;

import std.core;

export namespace Common
{
	template<typename Type>
	class MemoryPoolNode
	{
	public:
		using NodeIndex = std::size_t;

	private:
		union State
		{
			Type Element;
			struct
			{
				NodeIndex Next;
				NodeIndex Previous;
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

		NodeIndex GetPreviousIndex() const
		{
			return m_state.Meta.Previous;
		}
		void SetPreviousIndex(NodeIndex previous)
		{
			m_state.Meta.Previous = previous;
		}

		NodeIndex GetNextIndex() const
		{
			return m_state.Meta.Next;
		}
		void SetNextIndex(NodeIndex next)
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

	export template<typename T>
	class MemoryPoolIterator
	{
	public:
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using reference = T&;
		using pointer = T*;
		using iterator_category = std::forward_iterator_tag;

	private:
		using node_type = MemoryPoolNode<value_type>;

	public:
		MemoryPoolIterator() = default;
		~MemoryPoolIterator() = default;
		MemoryPoolIterator(const MemoryPoolIterator& other) = default;
		MemoryPoolIterator(MemoryPoolIterator&& other) noexcept = default;
		MemoryPoolIterator& operator=(const MemoryPoolIterator& other) = default;
		MemoryPoolIterator& operator=(MemoryPoolIterator&& other) noexcept = default;

		MemoryPoolIterator(const typename std::deque<MemoryPoolIterator<T>>::iterator& begin, const typename std::deque<MemoryPoolIterator<T>>::iterator& end) :
			m_location(begin),
			m_end(end)
		{
		}

	public:

		bool operator==(const MemoryPoolIterator& other)
		{
			return m_location == other.m_location;
		}
		bool operator!=(const MemoryPoolIterator& other)
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
		MemoryPoolIterator& operator++()
		{
			do
			{
				++m_location;
			} while (m_location != m_end && !m_location->IsInitialized());

			return *this;
		}
		MemoryPoolIterator operator++(int)
		{
			MemoryPoolIterator other(*this);
			operator++();
			return other;
		}

	private:
		typename std::deque<node_type>::iterator m_location;
		typename std::deque<node_type>::iterator m_end;
	};

	export template <class T>
	class MemoryPool
	{
	public:
		using Iterator = MemoryPoolIterator<T>;

	private:		
		using Node = MemoryPoolNode<T>;
		using NodeIndex = typename MemoryPoolNode<T>::NodeIndex;

	public:
		MemoryPool() :
			m_frontIndex(std::numeric_limits<NodeIndex>::max()),
			m_size(0)
		{
		}

	public:
		NodeIndex Push()
		{
			// Try to get the front node:
			NodeIndex index;
			auto node = PopFront(index);

			// If a node is not available:
			if (!node)
			{
				// Emplace node at the end of the deque:
				m_storage.emplace_back();

				// Get emplaced node:
				index = static_cast<NodeIndex>(m_storage.size() - 1);
				node = &m_storage.back();
			}

			// Signal that node will be used to contain data:
			node->Initialize();

			// Increase size:
			++m_size;

			// Return pointer to element:
			return index;
		}
		void Pop(NodeIndex index)
		{
			auto node = &m_storage[index];

			// Decrease size:
			--m_size;

			// Signal that the node will be used to store a reference to the next node:
			node->Shutdown();

			// If node is not at the end, set as the front node:
			if (index < static_cast<NodeIndex>(m_storage.size() - 1))
			{
				PushFront(index);
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

		Iterator Begin()
		{
			return iterator(m_storage.begin(), m_storage.end());
		}
		Iterator End()
		{
			return iterator(m_storage.end(), m_storage.end());
		}

		std::size_t GetSize() const
		{
			return m_size;
		}

	public:
		const T& operator[](NodeIndex index) const
		{
			return *m_storage[index].GetElement();
		}
		T& operator[](NodeIndex index)
		{
			return *m_storage[index].GetElement();
		}

	private:
		void PushFront(NodeIndex nodeIndex)
		{
			if (nodeIndex != std::numeric_limits<NodeIndex>::max())
			{
				Node* node = &m_storage[nodeIndex];

				// Set previous to null:
				node->SetPreviousIndex(std::numeric_limits<NodeIndex>::max());

				// Set the front node as the next of the pushed node:
				node->SetNextIndex(m_frontIndex);
			}

			if (m_frontIndex != std::numeric_limits<NodeIndex>::max())
			{
				// Set the previous node of the old front:
				m_storage[m_frontIndex].SetPreviousIndex(nodeIndex);
			}

			// Set the pushed node as the front:
			m_frontIndex = nodeIndex;
		}
		Node* PopFront(NodeIndex& index)
		{
			Node* node = nullptr;

			// If a node is available:
			if (m_frontIndex != std::numeric_limits<NodeIndex>::max())
			{
				// Get node at the front:
				node = &m_storage[m_frontIndex];

				// Assign new front:
				m_frontIndex = m_storage[m_frontIndex].GetNextIndex();

				// Set previous to null:
				if (m_frontIndex != std::numeric_limits<NodeIndex>::max())
					m_storage[m_frontIndex].SetPreviousIndex(std::numeric_limits<NodeIndex>::max());
			}

			return node;
		}
		void RemoveAt(typename std::deque<Node>::reverse_iterator location)
		{
			// Get previous node index:
			auto previousIndex = location->GetPreviousIndex();

			// Get next node index:
			auto nextIndex = location->GetNextIndex();

			if (previousIndex != std::numeric_limits<NodeIndex>::max())
				m_storage[previousIndex].SetNextIndex(nextIndex);

			if (nextIndex != std::numeric_limits<NodeIndex>::max())
				m_storage[nextIndex].SetPreviousIndex(previousIndex);
		}

	private:
		std::deque<Node> m_storage;
		NodeIndex m_frontIndex;
		std::size_t m_size;
	};
}
