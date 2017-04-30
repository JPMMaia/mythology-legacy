#pragma once

#include <array>

#include "MemoryPoolElement.h"

namespace Common
{
	template<typename Type, std::size_t Size>
	class MemoryPool
	{
	public:
		using ContainerType = std::array<MemoryPoolElement<Type>, Size>;
		using ContainerIterator = typename ContainerType::iterator;

	public:
		MemoryPool()
		{
			m_activeElements = 0;
			m_firstAvailable = &m_pool[0];

			// Initialize a list of available elements:
			for (std::size_t i = 0; i < m_pool.size() - 1; i++)
				m_pool[i].SetNext(&m_pool[i + 1]);

			// Set the last element to end with a nullptr:
			m_pool[Size - 1].SetNext(nullptr);
		}
		~MemoryPool() = default;

		template<typename... ArgumentsType>
		Type& NewElement(ArgumentsType&&... arguments)
		{
			// Make sure that the pool isn't full:
			if (m_firstAvailable == nullptr)
				throw std::out_of_range("Memory pool is full!");

			++m_activeElements;

			// Remove new element from the list of available elements:
			auto& newElement = *m_firstAvailable;
			m_firstAvailable = newElement.GetNext();

			// Initialize element:
			newElement.Initialize(std::forward<ArgumentsType>(arguments)...);

			// Return pointer to element:
			return newElement.GetElement();
		}

		void DeleteElement(Type& element)
		{
			// Calculating the index of the element:
			auto baseAddress = reinterpret_cast<std::size_t>(&m_pool[0]);
			auto elementAddress = reinterpret_cast<std::size_t>(&element);
			auto index = (elementAddress - baseAddress) / sizeof(MemoryPoolElement<Type>);

			auto& poolElement = m_pool[index];
			if (poolElement.IsInitialized())
			{
				poolElement.Shutdown(m_firstAvailable);
				m_firstAvailable = &poolElement;

				--m_activeElements;
			}
		}

		Type& operator[](std::size_t index)
		{
			return m_pool[index].GetElement();
		}

		std::size_t GetActiveElements() const
		{
			return m_activeElements;
		}
		static constexpr std::size_t GetMaxSize()
		{
			return Size;
		}

		ContainerIterator Begin()
		{
			return m_pool.begin();
		}
		ContainerIterator End()
		{
			return m_pool.end();
		}

	private:
		ContainerType m_pool;
		std::size_t m_activeElements;

		MemoryPoolElement<Type>* m_firstAvailable;
	};
}